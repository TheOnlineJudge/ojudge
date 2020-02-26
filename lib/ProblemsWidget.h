/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef PROBLEMSWIDGET_H
#define PROBLEMSWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WSortFilterProxyModel.h>
#include <Wt/WTreeView.h>
#include <Wt/WTableView.h>

class ViewModels;

class ProblemsWidget : public Wt::WContainerWidget {
public:
ProblemsWidget(ViewModels *viewModels);

private:
std::shared_ptr<Wt::WSortFilterProxyModel> proxyModel_;
ViewModels *viewModels_;
void categoryClicked(Wt::WModelIndex modelIndex, Wt::WMouseEvent mouseEvent);
void problemClicked(Wt::WModelIndex modelIndex, Wt::WMouseEvent mouseEvent);

};

#endif // PROBLEMSWIDGET_H
