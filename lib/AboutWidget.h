/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>

class AboutWidget : public Wt::WContainerWidget {
public:
AboutWidget();
void login(Wt::Auth::Login& login);
void logout();
};

#endif // ABOUTWIDGET_H
