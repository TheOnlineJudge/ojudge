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

#include <Wt/Auth/Login.h>
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
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
ViewModels *viewModels_;
DBModel *dbmodel_;
Wt::Signal<Wt::Auth::Login&>& loginSignal() {
	return loginSignal_;
}
Wt::Signal<>& logoutSignal() {
	return logoutSignal_;
}

Wt::Signal<Wt::Auth::Login&> loginSignal_;
Wt::Signal<> logoutSignal_;


class AdminCategoryWidget : public Wt::WContainerWidget {
public:
AdminCategoryWidget(const std::shared_ptr<CategoryModel> catmodel);
void login(Wt::Auth::Login& login);
void logout();

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
void login(Wt::Auth::Login& login);
void logout();
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
void login(Wt::Auth::Login& login);
void logout();

private:
Wt::WVBoxLayout *mainLayout_;
};

class AdminLanguageWidget : public Wt::WContainerWidget {
public:
AdminLanguageWidget();
void login(Wt::Auth::Login& login);
void logout();

private:
Wt::WVBoxLayout *mainLayout_;
};

class AdminProblemWidget : public Wt::WContainerWidget {
public:
AdminProblemWidget(ViewModels *viewModels, DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

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
void login(Wt::Auth::Login& login);
void logout();

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

class AdminGeneralSettingsWidget : public Wt::WTemplate {
public:
AdminGeneralSettingsWidget(DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

private:
DBModel *dbmodel_;
};

class AdminMailSettingsWidget : public Wt::WTemplate {
public:
AdminMailSettingsWidget();
void login(Wt::Auth::Login& login);
void logout();

private:
DBModel *dbmodel_;
};

class AdminContactSettingsWidget : public Wt::WTemplate {
public:
AdminContactSettingsWidget(DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

private:
DBModel *dbmodel_;
};

class AdminFooterSettingsWidget : public Wt::WTemplate {
public:
AdminFooterSettingsWidget(DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

private:
DBModel *dbmodel_;
};

class AdminUserWidget : public Wt::WContainerWidget {
public:
AdminUserWidget();
void login(Wt::Auth::Login& login);
void logout();

private:
Wt::WVBoxLayout *mainLayout_;
};

};

#endif // ADMINWIDGET_H
