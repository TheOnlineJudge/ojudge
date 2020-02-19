/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WServer.h>
#include "../ojudgeApp.h"
#include "../datastore/CategoryStore.h"
#include "CategoryModel.h"

using namespace Wt;

CategoryModel::CategoryModel(CategoryStore *categoryStore) : WAbstractItemModel(), categoryStore_(categoryStore) {

	std::map<int,CategoryData> categories = categoryStore_->getCategories();

	childIndex_[1] = 0;
	dbToInternal_[1] = getTreeId(-1,-1,1,categories[1].categories);

	for(auto& item : categories) {
		if(item.first>1) {
			childIndex_[item.first] = 0;
			dbToInternal_[item.first] = getTreeId(dbToInternal_.at(item.second.parent),childIndex_[item.second.parent]++,item.first,item.second.categories);
		}
	}
}

void CategoryModel::addCategory(std::string title, WModelIndex& parent) {

	int parentId = (parent.isValid()?cpp17::any_cast<int>(parent.data(CategoryModel::CategoryIdRole)):1);
	categoryStore_->addCategory(title,parent,childIndex_.at(parentId));
}

void CategoryModel::insertCategory(int row, const WModelIndex& parent, int categoryId) {

	int parentId = (parent.isValid()?cpp17::any_cast<int>(parent.data(CategoryModel::CategoryIdRole)):1);

	childIndex_[categoryId] = 0;
	dbToInternal_[categoryId] = getTreeId(dbToInternal_.at(parentId),childIndex_[parentId]++,categoryId,0);
	Tree& item = (parent.isValid()?treeData_[getTreeId(parent.internalId(),parent.row())]:treeData_[0]);

	beginInsertRows(parent,row,row);
	item.addRow();
	endInsertRows();
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

	if (!index.isValid())
		return cpp17::any();

	const Tree& item = treeData_[getTreeId(index.internalId(),index.row())];
	int category = item.categoryId();

	if(role == ItemDataRole::Display) {
		switch(index.column()) {
		case 0:
			return categoryStore_->getCategory(category).title;
		case 1:
			return categoryStore_->getCategory(category).problems;
		case 2:
			return categoryStore_->getCategory(category).categories;
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

void CategoryModel::Tree::addRow() {
	rowCount_++;
}

int CategoryModel::getTreeId(int parentId, int childIndex, int categoryId, int rowCount) const {

	ChildIndex index(parentId, childIndex);

	ChildPointerMap::const_iterator i = childPointer_.find(index);
	if(i == childPointer_.end()) {
		treeData_.push_back(Tree(parentId, childIndex, categoryId, rowCount));
		int result = treeData_.size() - 1;
		childPointer_[index] = result;
		return result;
	} else {
		return i->second;
	}
}
