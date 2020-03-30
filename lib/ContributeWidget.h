/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef CONTRIBUTEWIDGET_H
#define CONTRIBUTEWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>

class ContributeWidget : public Wt::WContainerWidget {
public:
ContributeWidget();
void login(Wt::Auth::Login& login);
void logout();

};

#endif // CONTRIBUTEWIDGET_H
