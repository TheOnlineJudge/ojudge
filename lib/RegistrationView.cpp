/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WLineEdit.h>
#include "RegistrationView.h"
#include "dbmodel/DBModel.h"

using namespace Wt;

RegistrationView::RegistrationView(Session &session,Auth::AuthWidget *authWidget) : Auth::RegistrationWidget(authWidget),session_(session) {
	setTemplateText(tr("template.registration"));
}

void RegistrationView::login(Auth::Login& login) {

}

void RegistrationView::logout() {

}

std::unique_ptr<WWidget> RegistrationView::createFormWidget(WFormModel::Field field) {
	return Auth::RegistrationWidget::createFormWidget(field);
}

bool RegistrationView::validate() {
	bool result = Auth::RegistrationWidget::validate();

	return result;
}

void RegistrationView::registerUserDetails(Auth::User& user) {

	session_.createUserData(user);
}
