/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WAbstractItemModel.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WTreeView.h>
#include <Wt/WDialog.h>
#include <Wt/WLineEdit.h>
#include <Wt/WCheckBox.h>
#include <Wt/WWidget.h>
#include <Wt/WTableView.h>
#include <Wt/WAbstractItemDelegate.h>
#include <Wt/WSortFilterProxyModel.h>
#include <Wt/WFileUpload.h>
#include "viewmodels/ViewModels.h"
#include "dbmodel/DBModel.h"

class Session;

class AdminWidget : public Wt::WContainerWidget {
public:
AdminWidget(Session *session, ViewModels *viewModels, DBModel *dbmodel);

private:
Session *session_;
ViewModels *viewModels_;
DBModel *dbmodel_;

class AdminCategoryWidget : public Wt::WContainerWidget {
public:
AdminCategoryWidget(const std::shared_ptr<CategoryModel> catmodel);

private:
const std::shared_ptr<CategoryModel> catmodel_;
Wt::WVBoxLayout *mainLayout_;
Wt::WTreeView *treeWidget_;
Wt::WDialog *addEditDialog_;
Wt::WCheckBox *rootCheckBox_;
Wt::WTreeView *selectParent_;
Wt::WLineEdit *title_;
void showAddEditDialog(const Wt::WModelIndex& index = Wt::WModelIndex());
void addEditDialogDone(Wt::DialogCode code);

class AdminActionsDelegate : public Wt::WAbstractItemDelegate {
public:
AdminActionsDelegate();
virtual std::unique_ptr<Wt::WWidget> update(Wt::WWidget *widget, const Wt::WModelIndex& index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) override;
Wt::Signal<const Wt::WModelIndex&>& editCategory() {
	return editCategory_;
}
Wt::Signal<const Wt::WModelIndex&>& deleteCategory() {
	return deleteCategory_;
}
Wt::Signal<const Wt::WModelIndex&,bool>& moveCategory() {
	return moveCategory_;
}

private:
struct WidgetRef {
	std::unique_ptr<Wt::WWidget> created;
	Wt::WWidget *w;
	WidgetRef(Wt::WWidget *widget) : w(widget) {
	}
};

Wt::Signal<const Wt::WModelIndex&> editCategory_;
Wt::Signal<const Wt::WModelIndex&> deleteCategory_;
Wt::Signal<const Wt::WModelIndex&,bool> moveCategory_;

};

};

class AdminContestWidget : public Wt::WContainerWidget {
public:
AdminContestWidget();

private:
Wt::WVBoxLayout *mainLayout_;
};

class AdminLanguageWidget : public Wt::WContainerWidget {
public:
AdminLanguageWidget();

private:
Wt::WVBoxLayout *mainLayout_;
};

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

class AdminSettingsWidget : public Wt::WContainerWidget {
public:
AdminSettingsWidget(DBModel *dbmodel);

private:
DBModel *dbmodel_;
};

class AdminUserWidget : public Wt::WContainerWidget {
public:
AdminUserWidget();

private:
Wt::WVBoxLayout *mainLayout_;
};

};

#endif // ADMINWIDGET_H
