/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef TEAMWIDGET_H
#define TEAMWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>

class TeamWidget : public Wt::WContainerWidget {
public:
TeamWidget();
void login(Wt::Auth::Login& login);
void logout();

};

#endif // TEAMWIDGET_H
