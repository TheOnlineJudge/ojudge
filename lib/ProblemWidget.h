/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef PROBLEMWIDGET_H
#define PROBLEMWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/Auth/Login.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WLink.h>
#include <Wt/WDialog.h>
#include "dbmodel/DBModel.h"
#include "widgets/OJCodeEditorWidget.h"

class ViewModels;
class OJProblemViewerWidget;
class OJCodeEditorWidget;
class ProblemDescriptionWidget;
class ProblemSidemenuWidget;
class ProblemStatisticsDialog;
class ProblemSubmissionDialog;

enum class ProblemWidgetDialog {
	Statistics,
	Submission
};

class ProblemWidget : public Wt::WContainerWidget {
public:
ProblemWidget(DBModel *dbmodel,ViewModels *viewModels, Session *session);
void login(Wt::Auth::Login& login);
void logout();
void setProblem(long long id);

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
Wt::WText *pageTitle_;
ProblemSidemenuWidget *menuWidget_;
ProblemDescriptionWidget *descriptionWidget_;
// Not for milestone 1.0.0 - ProblemDiscussionDialog *discussionDialog_;
ProblemStatisticsDialog *statisticsDialog_;
ProblemSubmissionDialog *submissionDialog_;
dbo::ptr<Problem> problemData_;
Session *session_;
Wt::Auth::Login *login_;
void showStatisticsDialog();
void showSubmissionDialog();
void closeStatisticsDialog();
void closeSubmissionDialog(Wt::DialogCode code);
Wt::Signal<Wt::Auth::Login&>& loginSignal() {
	return loginSignal_;
}
Wt::Signal<>& logoutSignal() {
	return logoutSignal_;
}

Wt::Signal<Wt::Auth::Login&> loginSignal_;
Wt::Signal<> logoutSignal_;
};

class ProblemDescriptionWidget : public Wt::WContainerWidget {
public:
ProblemDescriptionWidget(DBModel *dbmodel,ViewModels *viewModels);
void login(Wt::Auth::Login& login);
void logout();
void setProblem(dbo::ptr<Problem> problemData);

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
OJProblemViewerWidget *problemViewer_;
};

class ProblemSidemenuWidget : public Wt::WContainerWidget {
public:
ProblemSidemenuWidget(DBModel *dbmodel,ViewModels *viewModels);
void login(Wt::Auth::Login& login);
void logout();
void setProblem(dbo::ptr<Problem> problemData);
Wt::Signal<ProblemWidgetDialog>& showDialog() {
	return showDialog_;
}

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
Wt::WPushButton *downloadButton_;
Wt::Signal<ProblemWidgetDialog> showDialog_;
};

class ProblemStatisticsDialog : public Wt::WDialog {
public:
ProblemStatisticsDialog(DBModel *dbmodel,ViewModels *viewModels);
void login(Wt::Auth::Login& login);
void logout();
void setProblem(dbo::ptr<Problem> problemData);

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
};

class ProblemSubmissionDialog : public Wt::WDialog {
public:
ProblemSubmissionDialog(DBModel *dbmodel, ViewModels *viewModels, Session *session, Wt::Auth::Login *login);
void setProblem(dbo::ptr<Problem> problemData);
std::string code();
void setCode(std::string code);

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
OJCodeEditorWidget *codeEditor_;
Wt::Auth::Login *login_;
Session *session_;
void saveSettings(OJCodeEditorSettings& settings);
};

#endif // PROBLEMWIDGET_H
