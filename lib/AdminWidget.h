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
#include <Wt/WText.h>
#include "viewmodels/ViewModels.h"
#include "dbmodel/DBModel.h"
#include "datastore/DataStore.h"

class Session;
class OJColorPicker;

class AdminWidget : public Wt::WContainerWidget {
public:
AdminWidget(Session *session, ViewModels *viewModels, DBModel *dbmodel, DataStore *dataStore);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
ViewModels *viewModels_;
DBModel *dbmodel_;
DataStore *dataStore_;
Wt::WVBoxLayout *mainLayout_;
Wt::WContainerWidget *mainWidget_;
Wt::WText *mustLoginWidget_;
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
AdminLanguageWidget(ViewModels *viewModels);
void login(Wt::Auth::Login& login);
void logout();

private:
Wt::WVBoxLayout *mainLayout_;
Wt::WTableView *tableWidget_;
std::shared_ptr<Wt::WSortFilterProxyModel> proxyModel_;
ViewModels *viewModels_;
Wt::WDialog *addDialog_;
Wt::WLineEdit *name_;
Wt::WLineEdit *aceStyle_;
Wt::WLineEdit *compilerVersion_;
Wt::WFileUpload *compileScript_;
Wt::WFileUpload *linkScript_;
Wt::WFileUpload *runScript_;
Wt::WFileUpload *codeSkeleton_;
Wt::WText *errorMessage_;

void showAddEditDialog(const Wt::WModelIndex& index = Wt::WModelIndex());
void addDialogDone(Wt::DialogCode code);
class AdminActionsDelegate : public Wt::WAbstractItemDelegate {
public:
AdminActionsDelegate();
void login(Wt::Auth::Login& login);
void logout();

virtual std::unique_ptr<Wt::WWidget> update(Wt::WWidget *widget, const Wt::WModelIndex& index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) override;
Wt::Signal<const Wt::WModelIndex&>& editLanguage() {
	return editLanguage_;
}
Wt::Signal<const Wt::WModelIndex&>& deleteLanguage() {
	return deleteLanguage_;
}

private:
struct WidgetRef {
	std::unique_ptr<Wt::WWidget> created;
	Wt::WWidget *w;
	WidgetRef(Wt::WWidget *widget) : w(widget) {
	}
};

Wt::Signal<const Wt::WModelIndex&> editLanguage_;
Wt::Signal<const Wt::WModelIndex&> deleteLanguage_;

};

};

class AdminProblemWidget : public Wt::WContainerWidget {
public:
AdminProblemWidget(ViewModels *viewModels, ProblemStore *problemStore);
void login(Wt::Auth::Login& login);
void logout();

private:
Wt::WVBoxLayout *mainLayout_;
Wt::WTableView *tableWidget_;
std::shared_ptr<Wt::WSortFilterProxyModel> proxyModel_;
ViewModels *viewModels_;
ProblemStore *problemStore_;
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
AdminGeneralSettingsWidget(DataStore *dataStore);
void login(Wt::Auth::Login& login);
void logout();

private:
DataStore *dataStore_;
Wt::WLineEdit* siteTitle_;
Wt::WLineEdit* siteLogo_;
OJColorPicker* siteColor_;
Wt::WLineEdit* googleAnalytics_;
Wt::Auth::Login *login_;
bool siteTitleChanged_ = false;
bool siteLogoChanged_ = false;
bool siteColorChanged_ = false;
bool googleAnalyticsChanged_ = false;
void reset();
void resetClicked();
void applyClicked();
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
AdminUserWidget(ViewModels *viewModels, DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

private:
ViewModels *viewModels_;
DBModel *dbmodel_;
Wt::WVBoxLayout *mainLayout_;
Wt::WTableView *tableWidget_;
Wt::WLineEdit *userSelector_;
Wt::WDialog *editDialog_;
std::shared_ptr<Wt::WSortFilterProxyModel> proxyModel_;

void showEditDialog(const Wt::WModelIndex& index = Wt::WModelIndex());
void editDialogDone(Wt::DialogCode code);

class AdminActionsDelegate : public Wt::WAbstractItemDelegate {
public:
AdminActionsDelegate();
void login(Wt::Auth::Login& login);
void logout();

virtual std::unique_ptr<Wt::WWidget> update(Wt::WWidget *widget, const Wt::WModelIndex& index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) override;
Wt::Signal<const Wt::WModelIndex&>& editUser() {
	return editUser_;
}

private:
struct WidgetRef {
	std::unique_ptr<Wt::WWidget> created;
	Wt::WWidget *w;
	WidgetRef(Wt::WWidget *widget) : w(widget) {
	}
};

Wt::Signal<const Wt::WModelIndex&> editUser_;

};

};

};

#endif // ADMINWIDGET_H
