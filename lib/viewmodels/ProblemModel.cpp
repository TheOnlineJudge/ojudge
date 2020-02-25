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

using namespace Wt;

ProblemModel::ProblemModel(ProblemStore *problemStore) : WAbstractTableModel(), problemStore_(problemStore) {

}

void ProblemModel::addProblem(long long id, std::string title, const WModelIndex& parent) {
	problemStore_->addProblem(id,title,parent);
}

void ProblemModel::insertProblem(int row, const WModelIndex& parent) {

	beginInsertRows(parent,row,row);
	endInsertRows();

}

int ProblemModel::columnCount(const WModelIndex& parent) const {
	return 4;
}

int ProblemModel::rowCount(const WModelIndex& parent) const {
	return problemStore_->getProblems().size();
}

cpp17::any ProblemModel::data(const WModelIndex& index, ItemDataRole role) const {

	if(role == ItemDataRole::Display) {
		switch(index.column()) {
		case 0:
			return problemStore_->getProblem(index.row()).id;
		case 1:
			return problemStore_->getProblem(index.row()).title;
		case 2:
			return std::string("Who am I: " + std::to_string(problemStore_->getProblem(index.row()).id));
		case 3:
			return problemStore_->getProblem(index.row()).categories;
		}
	} else if(role == ItemDataRole::Decoration) {
		if(index.column()==0) {
			return std::string("images/problem.svg");
		}
	} else if(role == ItemDataRole::StyleClass) {
		if(index.column()==0) {
			return std::string("myAdminProblemTable");
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
