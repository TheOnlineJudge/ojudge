/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "../datastore/UserStore.h"

#include "UserModel.h"

using namespace Wt;

UserModel::UserModel(UserStore *userStore) : WAbstractTableModel(), userStore_(userStore) {

}

int UserModel::columnCount(const WModelIndex& parent) const {
	return 4;
}

int UserModel::rowCount(const WModelIndex& parent) const {
	return userStore_->getUsers().size();
}

cpp17::any UserModel::data(const WModelIndex& index, ItemDataRole role) const {

	if(role == ItemDataRole::Display) {
		switch(index.column()) {
		case 0:
			return userStore_->getUser(index.row()).id;
		case 1:
			return userStore_->getUser(index.row()).username;
		case 2:
			return userStore_->getUser(index.row()).firstname + " " + userStore_->getUser(index.row()).lastname;
		case 3:
			return userStore_->getUser(index.row()).email;
		}
	} else if(role == ItemDataRole::StyleClass) {
		if(index.column()==0) {
			return std::string("oj-admin-user-table-item");
		}
	}

	return cpp17::any();

}

cpp17::any UserModel::headerData(int section, Orientation orientation, ItemDataRole role) const {
	if(orientation == Orientation::Horizontal && role == ItemDataRole::Display) {
		switch(section) {
		case 0:
			return std::string("ID");
		case 1:
			return std::string("Username");
		case 2:
			return std::string("Name");
		case 3:
			return std::string("eMail");
		default:
			return cpp17::any();
		}
	}

	return cpp17::any();
}

WModelIndex UserModel::index(int row, int column, const WModelIndex& parent) const {
	return createIndex(row,column,parent.internalId());
}
