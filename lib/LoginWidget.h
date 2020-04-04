/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/AuthModel.h>
#include <Wt/Auth/Login.h>
#include "AuthWidget.h"

class Session;

class LoginWidget : public Wt::WContainerWidget {
public:
LoginWidget(Session *session);

private:
Session *session_;
std::unique_ptr<AuthWidget> authWidget_;
};

#endif // LOGINWIDGET_H
