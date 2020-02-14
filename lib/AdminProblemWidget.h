/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef ADMINPROBLEMWIDGET_H
#define ADMINPROBLEMWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WTableView.h>
#include <Wt/WSortFilterProxyModel.h>
#include <Wt/WLineEdit.h>
#include "viewmodels/ViewModels.h"

class AdminProblemWidget : public Wt::WContainerWidget {
public:
AdminProblemWidget(ViewModels *viewModels);

private:
Wt::WVBoxLayout *mainLayout_;
Wt::WTableView *tableWidget_;
std::shared_ptr<Wt::WSortFilterProxyModel> proxyModel_;
ViewModels *viewModels_;
Wt::WLineEdit *problemSelector_;
void problemSelectorSlot();
};

#endif // ADMINPROBLEMWIDGET_H
