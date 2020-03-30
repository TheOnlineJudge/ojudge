/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef LANGUAGESWIDGET_H
#define LANGUAGESWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>

class LanguagesWidget : public Wt::WContainerWidget {
public:
LanguagesWidget();
void login(Wt::Auth::Login& login);
void logout();

};

#endif // LANGUAGESWIDGET_H
