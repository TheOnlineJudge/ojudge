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
#include <Wt/WEvent.h>
#include "dbmodel/DBModel.h"
#include "AuthWidget.h"
#include "LoginWidget.h"

using namespace Wt;

LoginWidget::LoginWidget(Session *session) : session_(session) {

	auto authWidget = cpp14::make_unique<AuthWidget>(*session_);
	authWidget->model()->addPasswordAuth(&Session::passwordAuth());
	authWidget->model()->addOAuth(Session::oAuth());
	authWidget->setRegistrationEnabled(true);
	authWidget->processEnvironment();
	authWidget_ = authWidget.get();
	addWidget(std::move(authWidget));

	mouseWentOut().connect( [=] (WMouseEvent event) {
		if(event.widget().x < 0 || event.widget().x > 1279 || event.widget().y > 149) {
		        WAnimation anim(AnimationEffect::SlideInFromTop | AnimationEffect::Fade);
		        animateHide(anim);
		}
	});
}

AuthWidget* LoginWidget::authWidget() {
	return authWidget_;
}

void LoginWidget::login(Auth::Login& login) {

	WAnimation anim(AnimationEffect::SlideInFromTop | AnimationEffect::Fade);
	animateHide(anim);
}

void LoginWidget::logout() {

}
