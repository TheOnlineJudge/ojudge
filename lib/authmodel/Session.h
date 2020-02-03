/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#ifndef SESSION_H
#define SESSION_H

#include <Wt/Auth/Login.h>

#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/ptr.h>

#include "User.h"

using namespace Wt;

namespace dbo = Wt::Dbo;

typedef Auth::Dbo::UserDatabase<AuthInfo> UserDatabase;

class Session : public dbo::Session {
public:
	static void configureAuth();

	Session(const std::string &postgresDb);
	~Session();

	dbo::ptr<User> user();
	dbo::ptr<User> user(const Auth::User &user);

	Auth::AbstractUserDatabase &users();
	Auth::Login &login() { return login_; }

	static const Auth::AuthService &auth();
	static const Auth::PasswordService &passwordAuth();
	static const std::vector<const Auth::OAuthService*> oAuth();

private:
	std::unique_ptr<UserDatabase> users_;
	Auth::Login login_;
};

#endif // SESSION_H
