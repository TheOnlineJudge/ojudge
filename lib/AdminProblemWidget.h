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
#include <Wt/WDialog.h>
#include <Wt/WSortFilterProxyModel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTreeView.h>
#include <Wt/WFileUpload.h>
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
Wt::WDialog *addDialog_;
Wt::WLineEdit *newId_;
Wt::WLineEdit *newTitle_;
Wt::WFileUpload *newDescription_;
Wt::WTreeView *newCategories_;

void problemSelectorSlot();
void showAddDialog();
void addDialogDone(Wt::DialogCode code);
};

#endif // ADMINPROBLEMWIDGET_H
