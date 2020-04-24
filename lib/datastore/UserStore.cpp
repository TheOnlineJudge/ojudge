/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WServer.h>
#include <Wt/WApplication.h>
#include <mutex>
#include "../ojudgeApp.h"

#include "../dbmodel/DBModel.h"

#include "UserStore.h"

using namespace Wt;

UserStore::UserStore(DBModel *dbModel) : dbModel_(dbModel) {

	Users users = dbModel_->getUsers();
	Dbo::Transaction transaction = dbModel_->startTransaction();

	int row = 0;

	for(Users::const_iterator i = users.begin(); i != users.end(); i++) {
		dbo::ptr<User> user = *i;
		userDataIndex_[user->authInfo.id()] = row;
		userData_[row].id = user->authInfo.id();
		userData_[row].username = user->authInfo->identity("loginname").toUTF8();
		userData_[row].firstname = user->firstName.value_or("");
		userData_[row].lastname = user->lastName.value_or("");
		userData_[row].email = user->authInfo->email();
		userData_[row].status = user->authInfo->status();
		row++;
	}
}

const std::map<long long,UserData>& UserStore::getUsers() {
	return userData_;
}

const UserData& UserStore::getUser(long long id) {
	return userData_.at(id);
}
