/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "OJAuthModel.h"

using namespace Wt;

OJAuthModel::OJAuthModel(const Auth::AuthService& baseAuth, Auth::AbstractUserDatabase& users) : Auth::AuthModel(baseAuth, users) {

}

bool OJAuthModel::login(Auth::Login& login) {

	if (valid()) {
		auto self = shared_from_this();
		Auth::User user = users().findWithIdentity(Auth::Identity::LoginName,valueText(LoginNameField));
		cpp17::any v = value(RememberMeField);
		if (loginUser(login, user)) {
			reset();

			if (cpp17::any_has_value(v) && cpp17::any_cast<bool>(v) == true) setRememberMeCookie(user);

			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool OJAuthModel::loginUser(Auth::Login& login, Auth::User& user, Auth::LoginState state) {

	if (!user.isValid()) {
		return false;
	}

	if (user.status() == Auth::AccountStatus::Disabled) {
		setValidation(LoginNameField,WValidator::Result(ValidationState::Invalid,WString::tr("Wt.Auth.account-disabled")));

		login.login(user, Auth::LoginState::Disabled);

		return false;
	} else if (baseAuth()->emailVerificationRequired() && user.email().empty()) {
		setValidation(LoginNameField,WValidator::Result(ValidationState::Invalid,WString::tr("Wt.Auth.email-unverified")));

		login.login(user, Auth::LoginState::Disabled);

		return false;
	} else {
		login.login(user, state);

		return true;
	}
}
