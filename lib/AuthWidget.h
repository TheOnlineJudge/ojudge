/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef AUTHWIDGET_H
#define AUTHWIDGET_H

#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/Login.h>

using namespace Wt;

class Session;

class AuthWidget : public Auth::AuthWidget {
public:
AuthWidget(Session &session);
void login(Wt::Auth::Login& login);
void logout();
virtual std::unique_ptr<WWidget> createRegistrationView(const Auth::Identity& id) override;
void resetForm();

private:
Session &session_;
};

#endif // AUTHWIDGET_H
