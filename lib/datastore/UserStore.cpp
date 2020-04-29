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
		userData_[row].role = user->role;
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

cpp17::any UserStore::getUserSetting(const Auth::User& user, std::string setting) {
	Dbo::Transaction t = dbModel_->startTransaction();
	dbo::ptr<UserSettings> settings = dbModel_->getUserSettings(user);

	if(setting == "editor_fontsize") {
		return settings->editor_fontsize.value_or(16);
	} else if(setting == "editor_indent") {
		return settings->editor_indent.value_or(4);
	} else if(setting == "editor_wrap") {
		return settings->editor_wrap.value_or(true);
	} else if(setting == "editor_theme") {
		return settings->editor_theme.value_or("textmate");
	}

	return cpp17::any();
}

void UserStore::setUserSetting(const Auth::User& user, std::string setting, cpp17::any value) {
	Dbo::Transaction t = dbModel_->startTransaction();
	dbo::ptr<UserSettings> settings = dbModel_->getUserSettings(user);

	if(setting == "editor_fontsize") {
		settings.modify()->editor_fontsize = cpp17::any_cast<int>(value);
		return;
	} else if(setting == "editor_indent") {
		settings.modify()->editor_indent = cpp17::any_cast<int>(value);
		return;
	} else if(setting == "editor_wrap") {
		settings.modify()->editor_wrap = cpp17::any_cast<bool>(value);
		return;
	} else if(setting == "editor_theme") {
		settings.modify()->editor_theme = cpp17::any_cast<std::string>(value);
		return;
	}

	return;
}
