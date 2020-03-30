/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WFont.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WTable.h>
#include "ProfileWidget.h"

using namespace Wt;

ProfileWidget::ProfileWidget(Session *session, DBModel *dbmodel) : session_(session), dbmodel_(dbmodel) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	auto pageTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Profile"),0);
	pageTitle->addStyleClass("oj-pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);
	auto menuLayout = mainWidget->setLayout(cpp14::make_unique<WHBoxLayout>());

	auto mainStack = menuLayout->addWidget(cpp14::make_unique<WStackedWidget>(),1);
	auto menuWidget = menuLayout->insertWidget(0,cpp14::make_unique<WMenu>(mainStack),0);
	menuWidget->addStyleClass("flex-column");
	menuWidget->setWidth(200);

	auto accountWidget = cpp14::make_unique<AccountWidget>(session_,dbmodel_);
	loginSignal().connect(accountWidget.get(),&AccountWidget::login);
	logoutSignal().connect(accountWidget.get(),&AccountWidget::logout);
	auto accountItem = menuWidget->addItem("Account",std::move(accountWidget));

	auto securityWidget = cpp14::make_unique<SecurityWidget>(session_,dbmodel_);
	loginSignal().connect(securityWidget.get(),&SecurityWidget::login);
	logoutSignal().connect(securityWidget.get(),&SecurityWidget::logout);
	auto securityItem = menuWidget->addItem("Security",std::move(securityWidget));

	auto notificationsWidget = cpp14::make_unique<NotificationsWidget>(session_,dbmodel_);
	loginSignal().connect(notificationsWidget.get(),&NotificationsWidget::login);
	logoutSignal().connect(notificationsWidget.get(),&NotificationsWidget::logout);
	auto notificationsItem = menuWidget->addItem("Notifications",std::move(notificationsWidget));

	auto editorWidget = cpp14::make_unique<EditorWidget>(session_,dbmodel_);
	loginSignal().connect(editorWidget.get(),&EditorWidget::login);
	logoutSignal().connect(editorWidget.get(),&EditorWidget::logout);
	auto editorItem = menuWidget->addItem("Code Editor",std::move(editorWidget));
}

void ProfileWidget::login(Auth::Login& login) {
	loginSignal().emit(login);
}

void ProfileWidget::logout() {
	logoutSignal().emit();
}

ProfileWidget::AccountWidget::AccountWidget(Session *session, DBModel *dbmodel) : session_(session), dbmodel_(dbmodel) {

	addWidget(cpp14::make_unique<WText>("Account"));

	auto accountTable = addWidget(cpp14::make_unique<WTable>());

	accountTable->elementAt(0,0)->addWidget(cpp14::make_unique<WText>("User ID"));
	//accountTable->elementAt(0,1)->addWidget(cpp14::make_unique<WText>(session_->login().user().id()));

	accountTable->elementAt(1,0)->addWidget(cpp14::make_unique<WText>("Username"));
	//accountTable->elementAt(1,1)->addWidget(cpp14::make_unique<WText>(session_->login().user().identity("loginname")));

	accountTable->elementAt(2,0)->addWidget(cpp14::make_unique<WText>("eMail"));
	//accountTable->elementAt(2,1)->addWidget(cpp14::make_unique<WText>(session_->login().user().email()));
}

void ProfileWidget::AccountWidget::login(Auth::Login& login) {

}

void ProfileWidget::AccountWidget::logout() {

}

ProfileWidget::SecurityWidget::SecurityWidget(Session *session, DBModel *dbmodel) : session_(session), dbmodel_(dbmodel) {

	addWidget(cpp14::make_unique<WText>("Security"));

	auto securityTable = addWidget(cpp14::make_unique<WTable>());

	securityTable->elementAt(0,0)->addWidget(cpp14::make_unique<WText>("Two-factor authentication"));
	securityTable->elementAt(0,0)->addWidget(cpp14::make_unique<WText>("To protect your account, a security code is required in addition to your password."));
	securityTable->elementAt(0,1)->addWidget(cpp14::make_unique<WText>("ON/OFF"));
	securityTable->elementAt(0,2)->addWidget(cpp14::make_unique<WText>("Enable/Disable"));

	securityTable->elementAt(1,0)->addWidget(cpp14::make_unique<WText>("Password"));
	securityTable->elementAt(1,1)->addWidget(cpp14::make_unique<WText>("dummytext"));
	securityTable->elementAt(1,2)->addWidget(cpp14::make_unique<WText>("Change"));

}

void ProfileWidget::SecurityWidget::login(Auth::Login& login) {

}

void ProfileWidget::SecurityWidget::logout() {

}

ProfileWidget::NotificationsWidget::NotificationsWidget(Session *session, DBModel *dbmodel) : session_(session), dbmodel_(dbmodel) {

	addWidget(cpp14::make_unique<WText>("Notifications"));

	auto notificationsTable = addWidget(cpp14::make_unique<WTable>());

	notificationsTable->elementAt(0,0)->addWidget(cpp14::make_unique<WText>("eMail notifications"));

	notificationsTable->elementAt(1,0)->addWidget(cpp14::make_unique<WText>("Browser notifications"));

	notificationsTable->elementAt(2,0)->addWidget(cpp14::make_unique<WText>("App notifications"));

}

void ProfileWidget::NotificationsWidget::login(Auth::Login& login) {

}

void ProfileWidget::NotificationsWidget::logout() {

}

ProfileWidget::EditorWidget::EditorWidget(Session *session, DBModel *dbmodel) : session_(session), dbmodel_(dbmodel) {

	addWidget(cpp14::make_unique<WText>("Editor settings"));

}

void ProfileWidget::EditorWidget::login(Auth::Login& login) {

}

void ProfileWidget::EditorWidget::logout() {

}
