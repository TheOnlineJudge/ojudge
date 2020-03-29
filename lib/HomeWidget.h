/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>

class HomeWidget : public Wt::WContainerWidget {

public:
HomeWidget();
void login(Wt::Auth::Login& login);
void logout();

};

#endif // HOMEWIDGET_H
