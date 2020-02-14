/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef ADMINCONTESTWIDGET_H
#define ADMINCONTESTWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>

class AdminContestWidget : public Wt::WContainerWidget {
public:
AdminContestWidget();

private:
Wt::WVBoxLayout *mainLayout_;
};

#endif // ADMINCONTESTWIDGET_H
