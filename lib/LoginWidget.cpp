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
#include <Wt/WBorderLayout.h>
#include "dbmodel/DBModel.h"
#include "AuthWidget.h"
#include "LoginWidget.h"

using namespace Wt;

LoginWidget::LoginWidget(Session *session) : session_(session) {

	auto wrapLayout = setLayout(cpp14::make_unique<WBorderLayout>());

	authWidget_ = cpp14::make_unique<AuthWidget>(*session_);
	authWidget_->model()->addPasswordAuth(&Session::passwordAuth());
	authWidget_->model()->addOAuth(Session::oAuth());
	authWidget_->setRegistrationEnabled(true);
	authWidget_->processEnvironment();

	auto item = cpp14::make_unique<WContainerWidget>();
	item->setHeight(WLength(100));
	wrapLayout->addWidget(std::move(item),LayoutPosition::North);

	item = cpp14::make_unique<WContainerWidget>();
	item->setWidth(WLength(300));
	wrapLayout->addWidget(std::move(item),LayoutPosition::West);

	item = cpp14::make_unique<WContainerWidget>();
	item->setWidth(WLength(300));
	wrapLayout->addWidget(std::move(item),LayoutPosition::East);

	wrapLayout->addWidget(std::move(authWidget_),LayoutPosition::Center);

}
