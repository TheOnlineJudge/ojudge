/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef TUTORIALWIDGET_H
#define TUTORIALWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>

class TutorialWidget : public Wt::WContainerWidget {
public:
TutorialWidget();
void login(Wt::Auth::Login& login);
void logout();

};

#endif // TUTORIALWIDGET_H
