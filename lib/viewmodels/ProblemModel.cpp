/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "../datastore/ProblemStore.h"

#include "ProblemModel.h"
#include "CategoryModel.h"

using namespace Wt;

ProblemModel::ProblemModel(ProblemStore *problemStore, std::shared_ptr<CategoryModel> categoryModel) : WAbstractTableModel(), problemStore_(problemStore), categoryModel_(categoryModel) {

}

void ProblemModel::addProblem(long long id, std::string title, const WModelIndex& parent) {

	problemStore_->addProblem(id,title,parent);
}

void ProblemModel::insertProblem(int row, const WModelIndex& parent) {

	beginInsertRows(parent,row,row);
	endInsertRows();
}

void ProblemModel::setCategories(long long id, const WModelIndexSet& categories) {
	std::set<int> tmpCats;

	for(const WModelIndex& tmpIdx: categories) {
		tmpCats.insert(cpp17::any_cast<int>(tmpIdx.data(CategoryModel::CategoryIdRole)));
	}

	problemStore_->setCategories(id,tmpCats);
}

const WModelIndexSet ProblemModel::getCategories(long long id) {
	return categoryModel_->categoriesToIndexes(problemStore_->getCategories(id));
}

int ProblemModel::columnCount(const WModelIndex& parent) const {
	return 4;
}

int ProblemModel::rowCount(const WModelIndex& parent) const {
	return problemStore_->getProblems().size();
}

cpp17::any ProblemModel::data(const WModelIndex& index, ItemDataRole role) const {

	if(!problemStore_->getProblemByRow(index.row())) return cpp17::any();

	if(role == ItemDataRole::Display) {
		switch(index.column()) {
		case 0:
			return problemStore_->getProblemByRow(index.row()).value().id;
		case 1:
			return problemStore_->getProblemByRow(index.row()).value().title;
		case 2:
			return index.row();
		case 3:
			return problemStore_->getProblemByRow(index.row()).value().categories;
		}
	} else if(role == ItemDataRole::Decoration) {
		if(index.column()==0) {
			return std::string("images/problem.svg");
		}
	} else if(role == ItemDataRole::StyleClass) {
		if(index.column()==0) {
			return std::string("oj-admin-problem-table-item");
		}
	} else if(role == ProblemRowRole) {
		return index.row();
	}

	return cpp17::any();

}

cpp17::any ProblemModel::headerData(int section, Orientation orientation, ItemDataRole role) const {
	if(orientation == Orientation::Horizontal && role == ItemDataRole::Display) {
		switch(section) {
		case 0:
			return std::string("ID");
		case 1:
			return std::string("Title");
		case 2:
			return std::string("Actions");
		case 3:
			return std::string("Categories");
		default:
			return cpp17::any();
		}
	}

	return cpp17::any();
}

WModelIndex ProblemModel::index(int row, int column, const WModelIndex& parent) const {
	return createIndex(row,column,parent.internalId());
}
