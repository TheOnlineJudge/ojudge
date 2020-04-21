/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef USERSTORE_H
#define USERSTORE_H

#include <string>
#include <map>
#include <Wt/WModelIndex.h>
#include <Wt/Auth/User.h>

class DBModel;

struct UserData {
	long long id;
	std::string username;
	std::string firstname;
	std::string lastname;
	std::string email;
	Wt::Auth::AccountStatus status;
};


class UserStore {
public:
UserStore(DBModel *dbModel);
const std::map<long long,UserData>& getUsers();
const UserData& getUser(long long id);

private:
DBModel *dbModel_;
std::map<long long,UserData> userData_;
std::map<long long,long long> userDataIndex_;
};

#endif // USERSTORE_H
