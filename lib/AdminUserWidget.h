/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef ADMINUSERWIDGET_H
#define ADMINUSERWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>

class AdminUserWidget : public Wt::WContainerWidget {
public:
AdminUserWidget();

private:
Wt::WVBoxLayout *mainLayout_;
};

#endif // ADMINUSERWIDGET_H
