/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "../dbmodel/DBModel.h"
#include "ProblemModel.h"

using namespace Wt;

ProblemModel::ProblemModel(DBModel *dbmodel) : WAbstractTableModel(), dbmodel_(dbmodel) {

	refresh();
}

void ProblemModel::refresh() {

	layoutAboutToBeChanged().emit();

	problemData_.clear();

	populateData();

	layoutChanged().emit();

}

void ProblemModel::populateData() {

	Problems problems = dbmodel_->getProblems();
	Dbo::Transaction transaction = dbmodel_->startTransaction();

	int row = 0;

	for(Problems::const_iterator i = problems.begin(); i != problems.end(); i++) {
		dbo::ptr<Problem> problem = *i;
		problemData_[row].id = problem.id();
		problemData_[row].title = problem->title;
		for(Categories::const_iterator j = problem->categories.begin(); j != problem->categories.end(); j++) {
			dbo::ptr<Category> category = *j;
			problemData_[row].categories += std::string("#" + std::to_string(category.id()));
		}
		problemData_[row].categories += std::string("#");
		row++;
	}
}

int ProblemModel::columnCount(const WModelIndex& parent) const {
	return 4;
}

int ProblemModel::rowCount(const WModelIndex& parent) const {
	return problemData_.size();
}

cpp17::any ProblemModel::data(const WModelIndex& index, ItemDataRole role) const {

	if(role == ItemDataRole::Display) {
		switch(index.column()) {
		case 0:
			return problemData_.at(index.row()).id;
		case 1:
			return problemData_.at(index.row()).title;
		case 2:
			return std::string("Who am I: " + std::to_string(index.row()));
		case 3:
			return problemData_.at(index.row()).categories;
		}
	} else if(role == ItemDataRole::Decoration) {
		if(index.column()==0) {
			return std::string("images/problem.svg");
		}
	} else if(role == ItemDataRole::StyleClass) {
		if(index.column()==0) {
			return std::string("myAdminProblemTable");
		}
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
