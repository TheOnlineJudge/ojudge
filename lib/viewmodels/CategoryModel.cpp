/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "../dbmodel/DBModel.h"
#include "CategoryModel.h"

using namespace Wt;

CategoryModel::CategoryModel(DBModel *dbmodel) : WAbstractItemModel(), dbmodel_(dbmodel) {

	refresh();
}

void CategoryModel::refresh() {

	layoutAboutToBeChanged().emit();

	treeData_.clear();
	childPointer_.clear();
	categoryData_.clear();

	populateData();

	layoutChanged().emit();

}

void CategoryModel::populateData() {

	Categories categories = dbmodel_->getCategories();
	Dbo::Transaction transaction = dbmodel_->startTransaction();

	typedef std::vector< Dbo::ptr<Category> > tmpCategories;
	tmpCategories tmpcategories(categories.begin(), categories.end());

	std::map<int,int> childIndex;
	std::map<int,int> rowCount;

	for(tmpCategories::const_iterator i = tmpcategories.begin(); i != tmpcategories.end(); i++) {
		dbo::ptr<Category> category = *i;
		childIndex[category.id()] = 0;
		rowCount[category.id()] = 0;
	}

	for(tmpCategories::const_iterator i = tmpcategories.begin(); i != tmpcategories.end(); i++) {
		dbo::ptr<Category> category = *i;
		rowCount[category->parent.id()]++;
	}

	categoryData_[1].title = std::string("Root category");
	categoryData_[1].parent = 0;
	categoryData_[1].categories = rowCount.at(1);
	categoryData_[1].problems = 0;

	treeData_.push_back(Tree(-1,-1,1,rowCount.at(1)));

	for(tmpCategories::const_iterator i = tmpcategories.begin(); i != tmpcategories.end(); i++) {
		dbo::ptr<Category> category = *i;
		if(category.id()!=1) {
			categoryData_[category.id()].title = category->title;
			categoryData_[category.id()].parent = category->parent.id();
			categoryData_[category.id()].categories = rowCount.at(category.id());
			categoryData_[category.id()].problems = category->problems.size();

			getTreeId(category->parent.id()-1,childIndex[category->parent.id()]++,category.id(),rowCount.at(category.id()));
		}
	}
}

int CategoryModel::columnCount(const WModelIndex& parent) const {
	return 4;
}

int CategoryModel::rowCount(const WModelIndex& parent) const {
	int treeId;
	if(parent.isValid()) {
		if(parent.column() != 0) return 0;

		treeId = getTreeId(parent.internalId(),parent.row());
	} else {
		treeId = 0;
	}

	return treeData_[treeId].rowCount();
}

WModelIndex CategoryModel::parent(const WModelIndex& index) const {
	if(!index.isValid() || index.internalId() == 0) return WModelIndex();

	const Tree& item = treeData_[index.internalId()];
	return createIndex(item.index(), 0, item.parentId());
}

WModelIndex CategoryModel::index(int row, int column, const WModelIndex& parent) const {
	int parentId;

	if(!parent.isValid()) {
		parentId = 0;
	} else {
		int grandParentId = parent.internalId();
		parentId = getTreeId(grandParentId, parent.row());
	}

	return createIndex(row,column,parentId);
}

cpp17::any CategoryModel::data(const WModelIndex& index, ItemDataRole role) const {

	const Tree& item = treeData_[getTreeId(index.internalId(),index.row())];
	int category = item.categoryId();

	if(role == ItemDataRole::Display) {
		switch(index.column()) {
		case 0:
			return categoryData_.at(category).title;
		case 1:
			return categoryData_.at(category).problems;
		case 2:
			return categoryData_.at(category).categories;
		case 3:
			return std::string("Who am I: " + std::to_string(category));
		}
	} else if(role == ItemDataRole::Decoration) {
		if(index.column()==0) {
			return std::string("images/category.svg");
		}
	} else if(role == ItemDataRole::StyleClass) {
		if(index.column()==0) {
			return std::string("myTree");
		}
	} else if(role == CategoryIdRole) {
		return category;
	}

	return cpp17::any();

}

cpp17::any CategoryModel::headerData(int section, Orientation orientation, ItemDataRole role) const {
	if(orientation == Orientation::Horizontal && role == ItemDataRole::Display) {
		switch(section) {
		case 0:
			return std::string("Title");
		case 1:
			return std::string("Problems");
		case 2:
			return std::string("Categories");
		case 3:
			return std::string("Actions");
		default:
			return cpp17::any();
		}
	}

	return cpp17::any();
}

CategoryModel::Tree::Tree(int parentId, int index, int categoryId, int rowCount) : index_(parentId,index), categoryId_(categoryId), rowCount_(rowCount) {

}

int CategoryModel::Tree::parentId() const {
	return index_.parentId;
}

int CategoryModel::Tree::index() const {
	return index_.index;
}

int CategoryModel::Tree::categoryId() const {
	return categoryId_;
}

int CategoryModel::Tree::rowCount() const {
	return rowCount_;
}

int CategoryModel::getTreeId(int parentId, int childIndex, int categoryId, int rowCount) const {

	ChildIndex index(parentId, childIndex);

	ChildPointerMap::const_iterator i = childPointer_.find(index);
	if(i == childPointer_.end()) {
		const Tree& parentItem = treeData_[parentId];

		treeData_.push_back(Tree(parentId, childIndex, categoryId, rowCount));
		int result = treeData_.size() - 1;
		childPointer_[index] = result;
		return result;
	} else {
		return i->second;
	}
}
