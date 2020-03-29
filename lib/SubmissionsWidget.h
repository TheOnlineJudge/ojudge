/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef SUBMISSIONSWIDGET_H
#define SUBMISSIONSWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>

class SubmissionsWidget : public Wt::WContainerWidget {
public:
SubmissionsWidget();
void login(Wt::Auth::Login& login);
void logout();

};

#endif // SUBMISSIONSWIDGET_H
