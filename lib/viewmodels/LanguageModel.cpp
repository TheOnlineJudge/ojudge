/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "../datastore/LanguageStore.h"

#include "LanguageModel.h"

using namespace Wt;

LanguageModel::LanguageModel(LanguageStore *languageStore) : WAbstractTableModel(), languageStore_(languageStore) {

}

/*
   void ProblemModel::addProblem(long long id, std::string title, const WModelIndex& parent) {
        problemStore_->addProblem(id,title,parent);
   }

   void ProblemModel::insertProblem(int row, const WModelIndex& parent) {

        beginInsertRows(parent,row,row);
        endInsertRows();

   }
 */

int LanguageModel::columnCount(const WModelIndex& parent) const {
	return 4;
}

int LanguageModel::rowCount(const WModelIndex& parent) const {
	return languageStore_->getLanguages().size();
}

cpp17::any LanguageModel::data(const WModelIndex& index, ItemDataRole role) const {

	if(role == ItemDataRole::Display) {
		switch(index.column()) {
		case 0:
			return languageStore_->getLanguage(index.row()).id;
		case 1:
			return languageStore_->getLanguage(index.row()).name;
		case 2:
			return languageStore_->getLanguage(index.row()).compilerVersion;
		case 3:
			return index.row();
		}
	} else if(role == ItemDataRole::StyleClass) {
		if(index.column()==0) {
			return std::string("myAdminLanguageTable");
		}
	} else if(role == LanguageRowRole) {
		return index.row();
	}

	return cpp17::any();

}

cpp17::any LanguageModel::headerData(int section, Orientation orientation, ItemDataRole role) const {
	if(orientation == Orientation::Horizontal && role == ItemDataRole::Display) {
		switch(section) {
		case 0:
			return std::string("ID");
		case 1:
			return std::string("Name");
		case 2:
			return std::string("Version");
		case 3:
			return std::string("Actions");
		default:
			return cpp17::any();
		}
	}

	return cpp17::any();
}
