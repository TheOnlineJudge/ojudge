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
#include <Wt/Utils.h>
#include <mutex>
#include "../ojudgeApp.h"

#include "UserStore.h"
#include "../OJUtils.h"

std::mutex setUserSetting_mutex;

using namespace Wt;

UserStore::UserStore(DBModel *dbmodel) : dbmodel_(dbmodel) {

	Users users = dbmodel_->getUsers();
	Dbo::Transaction transaction = dbmodel_->startTransaction();

	int row = 0;

	for(Users::const_iterator i = users.begin(); i != users.end(); i++) {
		dbo::ptr<User> user = *i;
		userDataIndex_[user->authInfo.id()] = row;
		userData_[row].id = user->authInfo.id();
		userData_[row].username = user->authInfo->identity("loginname").toUTF8();
		userData_[row].firstname = user->firstName.value_or("");
		userData_[row].lastname = user->lastName.value_or("");
		userData_[row].email = user->authInfo->email();
		userData_[row].birthday = user->birthday.value_or(WDate());
		userData_[row].country = user->country.value_or("00");
		userData_[row].institution = user->institution.value_or("");
		userData_[row].uvaID = user->uvaID.value_or("");
		userData_[row].role = user->role;
		userData_[row].status = user->authInfo->status();
		userData_[row].avatarType = user->avatar->avatarType;
		row++;
	}
}

const std::map<long long,UserData>& UserStore::getUsers() {
	return userData_;
}

const UserData& UserStore::getUserByRow(long long id) {
	return userData_.at(id);
}

const UserData& UserStore::getUserById(long long id) {
	return userData_.at(userDataIndex_.at(id));
}

cpp17::any UserStore::getUserSetting(const Auth::User& user, UserSettingType setting) {
	Dbo::Transaction t = dbmodel_->startTransaction();

	dbo::ptr<User> userInDb = dbmodel_->getUser(std::stoll(user.id()));
	dbo::ptr<UserSettings> settings = userInDb->settings;

	switch(setting) {
	case UserSettingType::NotificationsEmailResults:
		return settings->notifications_email_results.value_or(true);
	case UserSettingType::NotificationsEmailContests:
		return settings->notifications_email_contests.value_or(true);
	case UserSettingType::NotificationsEmailGeneral:
		return settings->notifications_email_general.value_or(true);
	case UserSettingType::NotificationsEmailMessages:
		return settings->notifications_email_messages.value_or(true);
	case UserSettingType::NotificationsBrowserResults:
		return settings->notifications_browser_results.value_or(false);
	case UserSettingType::NotificationsBrowserContests:
		return settings->notifications_browser_contests.value_or(false);
	case UserSettingType::NotificationsBrowserGeneral:
		return settings->notifications_browser_general.value_or(false);
	case UserSettingType::NotificationsBrowserMessages:
		return settings->notifications_browser_messages.value_or(false);
	case UserSettingType::NotificationsTelegramResults:
		return settings->notifications_telegram_results.value_or(false);
	case UserSettingType::NotificationsTelegramContests:
		return settings->notifications_telegram_contests.value_or(false);
	case UserSettingType::NotificationsTelegramGeneral:
		return settings->notifications_telegram_general.value_or(false);
	case UserSettingType::NotificationsTelegramMessages:
		return settings->notifications_telegram_messages.value_or(false);
	case UserSettingType::EditorFontsize:
		return settings->editor_fontsize.value_or(16);
	case UserSettingType::EditorIndent:
		return settings->editor_indent.value_or(4);
	case UserSettingType::EditorWrap:
		return settings->editor_wrap.value_or(true);
	case UserSettingType::EditorTheme:
		return settings->editor_theme.value_or("textmate");
	case UserSettingType::TwoFAEnabled:
		return settings->twofa_enabled.value_or(false);
	case UserSettingType::TwoFASecret:
		return settings->twofa_secret.value_or(std::vector<unsigned char>());
	}

	return cpp17::any();
}

void UserStore::setUserSetting(const Auth::User& user, UserSettingType setting, cpp17::any value) {

	std::lock_guard<std::mutex> guard(setUserSetting_mutex);

	Dbo::Transaction t = dbmodel_->startTransaction();
	dbo::ptr<User> userInDb = dbmodel_->getUser(std::stoll(user.id()));
	dbo::ptr<UserSettings> settings = userInDb->settings;
	dbo::ptr<UserAvatar> avatar = userInDb->avatar;

	switch(setting) {
	case UserSettingType::NotificationsEmailResults:
		settings.modify()->notifications_email_results = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsEmailContests:
		settings.modify()->notifications_email_contests = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsEmailGeneral:
		settings.modify()->notifications_email_general = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsEmailMessages:
		settings.modify()->notifications_email_messages = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsBrowserResults:
		settings.modify()->notifications_browser_results = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsBrowserContests:
		settings.modify()->notifications_browser_contests = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsBrowserGeneral:	
		settings.modify()->notifications_browser_general = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsBrowserMessages:
		settings.modify()->notifications_browser_messages = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsTelegramResults:
		settings.modify()->notifications_telegram_results = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsTelegramContests:
		settings.modify()->notifications_telegram_contests = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsTelegramGeneral:
		settings.modify()->notifications_telegram_general = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::NotificationsTelegramMessages:
		settings.modify()->notifications_telegram_messages = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::EditorFontsize:
		settings.modify()->editor_fontsize = cpp17::any_cast<int>(value);
		break;
	case UserSettingType::EditorIndent:
		settings.modify()->editor_indent = cpp17::any_cast<int>(value);
		break;
	case UserSettingType::EditorWrap:
		settings.modify()->editor_wrap = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::EditorTheme:
		settings.modify()->editor_theme = cpp17::any_cast<std::string>(value);
		break;
	case UserSettingType::AvatarType:
		userData_[userDataIndex_[std::stoll(user.id())]].avatarType = cpp17::any_cast<AvatarType>(value);
		avatar.modify()->avatarType = cpp17::any_cast<AvatarType>(value);
		break;
	case UserSettingType::Avatar:
		avatar.modify()->avatar = cpp17::any_cast<std::vector<unsigned char> >(value);
		break;
	case UserSettingType::Firstname:
		userData_[userDataIndex_[std::stoll(user.id())]].firstname = cpp17::any_cast<std::string>(value);
		userInDb.modify()->firstName = cpp17::any_cast<std::string>(value);
		break;
	case UserSettingType::Lastname:
		userData_[userDataIndex_[std::stoll(user.id())]].lastname = cpp17::any_cast<std::string>(value);
		userInDb.modify()->lastName = cpp17::any_cast<std::string>(value);
		break;
	case UserSettingType::Birthday:
		userData_[userDataIndex_[std::stoll(user.id())]].birthday = cpp17::any_cast<WDate>(value);
		userInDb.modify()->birthday = cpp17::any_cast<WDate>(value);
		break;
	case UserSettingType::Country:
		userData_[userDataIndex_[std::stoll(user.id())]].country = cpp17::any_cast<std::string>(value);
		userInDb.modify()->country = cpp17::any_cast<std::string>(value);
		break;
	case UserSettingType::Institution:
		userData_[userDataIndex_[std::stoll(user.id())]].institution = cpp17::any_cast<std::string>(value);
		userInDb.modify()->institution = cpp17::any_cast<std::string>(value);
		break;
	case UserSettingType::TwoFAEnabled:
		settings.modify()->twofa_enabled = cpp17::any_cast<bool>(value);
		break;
	case UserSettingType::TwoFASecret:
		settings.modify()->twofa_secret = cpp17::any_cast<std::vector<unsigned char> >(value);
		break;
	}

	return;
}

const std::string UserStore::getAvatarLink(long long userId, AvatarType type, int size) {

	Dbo::Transaction t = dbmodel_->startTransaction();
	dbo::ptr<User> user = dbmodel_->getUser(userId);

	if(type==AvatarType::Set) {
		type = user->avatar->avatarType;
	}

        switch(type) {
        case AvatarType::Default:
                return "https://cdn.libravatar.org/avatar/" + OJUtils::bin_to_hex(Utils::md5(OJUtils::bin_to_hex(Utils::md5(user->authInfo->email())))) + "?s=" + std::to_string(size) + "&forcedefault=y&default=identicon";
        case AvatarType::Gravatar:
                return "https://cdn.libravatar.org/avatar/" + OJUtils::bin_to_hex(Utils::md5(user->authInfo->email())) + "?s=" + std::to_string(size) + "&default=identicon";
        case AvatarType::Custom:
                if(user->avatar->avatar.value_or(std::vector<unsigned char>()).size()) {
                        return "data:image/jpeg;base64,"+Utils::base64Encode(std::string(user->avatar->avatar.value().begin(),user->avatar->avatar.value().end()),false);
                }
                break;
        }

        return "https://www.libravatar.org/static/img/mm/" + std::to_string(size) + ".png";
}
