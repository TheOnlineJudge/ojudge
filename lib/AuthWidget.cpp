/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "AuthWidget.h"
#include "RegistrationView.h"
#include "dbmodel/DBModel.h"

AuthWidget::AuthWidget(Session &session) : Auth::AuthWidget(Session::auth(), session.users(), session.login()), session_(session) {

}

void AuthWidget::createLoginView() {

  setTemplateText(tr("template.login"));

  createPasswordLoginView();
  createOAuthLoginView();
}

std::unique_ptr<WWidget> AuthWidget::createRegistrationView(const Auth::Identity &id) {
	auto registrationView = cpp14::make_unique<RegistrationView>(session_, this);
	std::unique_ptr<Auth::RegistrationModel> model = createRegistrationModel();

	if(id.isValid()) {
		model->registerIdentified(id);
	}

	registrationView->setModel(std::move(model));
	return std::move(registrationView);
}

void AuthWidget::resetForm() {
	reset();
}
