/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/Auth/AuthService.h>
#include <Wt/Auth/HashFunction.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/PasswordStrengthValidator.h>
#include <Wt/Auth/PasswordVerifier.h>
#include <Wt/Auth/GoogleService.h>
#include <Wt/Auth/FacebookService.h>
#include <Wt/Auth/Dbo/AuthInfo.h>
#include <Wt/Auth/Dbo/UserDatabase.h>

#include <Wt/Dbo/backend/Postgres.h>

#include "Session.h"

namespace {
Auth::AuthService myAuthService;
Auth::PasswordService myPasswordService(myAuthService);
std::vector<std::unique_ptr<Auth::OAuthService> > myOAuthServices;
}

void Session::configureAuth() {
	myAuthService.setAuthTokensEnabled(true, "logincookie");
	myAuthService.setEmailVerificationEnabled(true);

	auto verifier = cpp14::make_unique<Auth::PasswordVerifier>();
	verifier->addHashFunction(cpp14::make_unique<Auth::BCryptHashFunction>(7));
	myPasswordService.setVerifier(std::move(verifier));
	myPasswordService.setAttemptThrottlingEnabled(true);
	myPasswordService.setStrengthValidator(cpp14::make_unique<Auth::PasswordStrengthValidator>());

	if (Auth::GoogleService::configured()) {
		myOAuthServices.push_back(cpp14::make_unique<Auth::GoogleService>(myAuthService));
	}

	if (Auth::FacebookService::configured()) {
		myOAuthServices.push_back(cpp14::make_unique<Auth::FacebookService>(myAuthService));
	}
}

Session::Session(const std::string &postgresDb) {
	auto connection = cpp14::make_unique<Dbo::backend::Postgres>(postgresDb);

	connection->setProperty("show-queries", "true");

	setConnection(std::move(connection));

	mapClass<User>("user");
	mapClass<AuthInfo>("auth_info");
	mapClass<AuthInfo::AuthIdentityType>("auth_identity");
	mapClass<AuthInfo::AuthTokenType>("auth_token");

	try {
		createTables();
		std::cerr << "Created database." << std::endl;
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "Using existing database." << std::endl;
	}

	users_ = cpp14::make_unique<UserDatabase>(*this);
}

Session::~Session() {

}

Auth::AbstractUserDatabase &Session::users() {
	return *users_;
}

dbo::ptr<User> Session::user() {
	if(login_.loggedIn()) {
		return user(login_.user());
	} else {
		return dbo::ptr<User>();
	}
}

dbo::ptr<User> Session::user(const Auth::User &authUser) {
	dbo::ptr<AuthInfo> authInfo = users_->find(authUser);

	dbo::ptr<User> user = authInfo->user();

	if(!user) {
		user = add(Wt::cpp14::make_unique<User>());
		authInfo.modify()->setUser(user);
	}

	return user;
}

const Auth::AuthService &Session::auth() {
	return myAuthService;
}

const Auth::PasswordService &Session::passwordAuth() {
	return myPasswordService;
}

const std::vector<const Auth::OAuthService *> Session::oAuth() {
	std::vector<const Auth::OAuthService *> result;
	for(auto &auth : myOAuthServices) {
		result.push_back(auth.get());
	}
	return result;
}
