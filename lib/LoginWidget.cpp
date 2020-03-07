/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/WVBoxLayout.h>
#include "authmodel/Session.h"
#include "AuthWidget.h"
#include "LoginWidget.h"

using namespace Wt;

LoginWidget::LoginWidget(Session *session) : session_(session) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());

	authWidget_ = cpp14::make_unique<AuthWidget>(*session_);
	authWidget_->addStyleClass("login");
	authWidget_->model()->addPasswordAuth(&Session::passwordAuth());
	authWidget_->model()->addOAuth(Session::oAuth());
	authWidget_->setRegistrationEnabled(true);
	authWidget_->processEnvironment();

	mainLayout->addWidget(std::move(authWidget_));

}
