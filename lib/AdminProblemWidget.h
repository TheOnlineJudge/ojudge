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
#include "dbmodel/DBModel.h"

#include <Wt/WAbstractItemDelegate.h>

class AdminProblemWidget : public Wt::WContainerWidget {
public:
AdminProblemWidget(ViewModels *viewModels, DBModel *dbmodel);

private:
Wt::WVBoxLayout *mainLayout_;
Wt::WTableView *tableWidget_;
std::shared_ptr<Wt::WSortFilterProxyModel> proxyModel_;
ViewModels *viewModels_;
DBModel *dbmodel_;
Wt::WLineEdit *problemSelector_;
Wt::WDialog *addDialog_;
Wt::WLineEdit *id_;
Wt::WLineEdit *title_;
Wt::WFileUpload *htmlDescription_;
Wt::WFileUpload *pdfDescription_;
Wt::WTreeView *categories_;

void problemSelectorSlot();
void showAddEditDialog(const Wt::WModelIndex& index = Wt::WModelIndex());
void addDialogDone(Wt::DialogCode code);

class AdminActionsDelegate : public Wt::WAbstractItemDelegate {
public:
AdminActionsDelegate();
virtual std::unique_ptr<Wt::WWidget> update(Wt::WWidget *widget, const Wt::WModelIndex& index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) override;
Wt::Signal<const Wt::WModelIndex&>& editProblem() {
	return editProblem_;
}
Wt::Signal<const Wt::WModelIndex&>& deleteProblem() {
	return deleteProblem_;
}

private:
struct WidgetRef {
	std::unique_ptr<Wt::WWidget> created;
	Wt::WWidget *w;
	WidgetRef(Wt::WWidget *widget) : w(widget) {
	}
};

Wt::Signal<const Wt::WModelIndex&> editProblem_;
Wt::Signal<const Wt::WModelIndex&> deleteProblem_;

};

};
#endif // ADMINPROBLEMWIDGET_H
