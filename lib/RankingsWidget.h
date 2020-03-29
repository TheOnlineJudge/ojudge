/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef RANKINGSWIDGET_H
#define RANKINGSWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>

class RankingsWidget : public Wt::WContainerWidget {
public:
RankingsWidget();
void login(Wt::Auth::Login& login);
void logout();

};

#endif // RANKINGSWIDGET_H
