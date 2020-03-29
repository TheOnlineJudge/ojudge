/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef FACTSWIDGET_H
#define FACTSWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>

class FactsWidget : public Wt::WContainerWidget {
public:
FactsWidget();
void login(Wt::Auth::Login& login);
void logout();

};

#endif // FACTSWIDGET_H
