/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "RegistrationView.h"
#include "authmodel/UserDetailsModel.h"

#include <Wt/WLineEdit.h>

using namespace Wt;

RegistrationView::RegistrationView(Session &session,Auth::AuthWidget *authWidget) : Auth::RegistrationWidget(authWidget),session_(session) {
	setTemplateText(tr("template.registration"));
	detailsModel_ = cpp14::make_unique<UserDetailsModel>(session_);

	updateView(detailsModel_.get());
}

std::unique_ptr<WWidget> RegistrationView::createFormWidget(WFormModel::Field field) {
//	if(field == UserDetailsModel::FavouritePetField) {
//		return cpp14::make_unique<WLineEdit>();
//	} else {
	return Auth::RegistrationWidget::createFormWidget(field);
//	}
}

bool RegistrationView::validate() {
	bool result = Auth::RegistrationWidget::validate();

	updateModel(detailsModel_.get());
	if(!detailsModel_->validate()) {
		result = false;
	}
	updateView(detailsModel_.get());

	return result;
}

void RegistrationView::registerUserDetails(Auth::User &user) {
	detailsModel_->save(user);
}
