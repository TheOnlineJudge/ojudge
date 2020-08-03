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
#include "../dbmodel/DBModel.h"

struct UserData {
	long long id;
	std::string username;
	std::string firstname;
	std::string lastname;
	std::string email;
	WDate birthday;
	std::string country;
	std::string institution;
	std::string uvaID;
	Role role;
	Wt::Auth::AccountStatus status;
	AvatarType avatarType;
};

enum class UserSettingType {
	NotificationsEmailResults,
	NotificationsEmailContests,
	NotificationsEmailGeneral,
	NotificationsEmailMessages,
	NotificationsBrowserResults,
	NotificationsBrowserContests,
	NotificationsBrowserGeneral,
	NotificationsBrowserMessages,
	NotificationsTelegramResults,
	NotificationsTelegramContests,
	NotificationsTelegramGeneral,
	NotificationsTelegramMessages,
	EditorFontsize,
	EditorIndent,
	EditorWrap,
	EditorTheme,
	EditorStyle,
	AvatarType,
	Avatar,
	Username,
	Email,
	Role,
	Firstname,
	Lastname,
	Birthday,
	Country,
	Institution,
	TwoFAEnabled,
	TwoFASecret,
	UvaID
};


class UserStore {
public:
UserStore(DBModel *dbmodel);
const std::map<long long,UserData>& getUsers();
const UserData& getUserByRow(long long id);
const UserData& getUserById(long long id);
cpp17::any getUserSetting(const Auth::User& user, UserSettingType setting);
cpp17::any getUserSetting(long long id, UserSettingType setting);
void setUserSetting(const Auth::User& user, UserSettingType setting, cpp17::any value);
void setUserSetting(long long user, UserSettingType setting, cpp17::any value);
const std::string getAvatarLink(long long userId, AvatarType type = AvatarType::Set, int size = 128);

private:
DBModel *dbmodel_;
std::map<long long,UserData> userData_;
std::map<long long,long long> userDataIndex_;
};

#endif // USERSTORE_H
