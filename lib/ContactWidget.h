/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>

class ContactWidget : public Wt::WContainerWidget {
public:
ContactWidget();
void login(Wt::Auth::Login& login);
void logout();
};

#endif // CONTACTWIDGET_H
