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
#include "widgets/OJCodeEditorWidget.h"
#include "widgets/OJRatingSetWidget.h"
#include "datastore/DataStore.h"
#include "datastore/ProblemStore.h"
#include "datastore/LanguageStore.h"
#include "datastore/SubmissionStore.h"
#include "dbmodel/DBModel.h"

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
ProblemWidget(ViewModels *viewModels, DataStore *dataStore);
void login(Wt::Auth::Login& login);
void logout();
void setProblem(long long id);

private:
ViewModels *viewModels_;
DataStore *dataStore_;
std::optional<ProblemData> problemData_;
Wt::WText *pageTitle_;
ProblemSidemenuWidget *menuWidget_;
ProblemDescriptionWidget *descriptionWidget_;
// Not for milestone 1.0.0 - ProblemDiscussionDialog *discussionDialog_;
ProblemStatisticsDialog *statisticsDialog_;
ProblemSubmissionDialog *submissionDialog_;
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
ProblemDescriptionWidget(ViewModels *viewModels, DataStore *dataStore);
void login(Wt::Auth::Login& login);
void logout();
void setProblem(long long id);

private:
ViewModels *viewModels_;
DataStore *dataStore_;
OJProblemViewerWidget *problemViewer_;
};

class ProblemSidemenuWidget : public Wt::WContainerWidget {
public:
ProblemSidemenuWidget(ViewModels *viewModels, DataStore *dataStore);
void login(Wt::Auth::Login& login);
void logout();
void setProblem(long long id);
Wt::Signal<ProblemWidgetDialog>& showDialog() {
	return showDialog_;
}

private:
ViewModels *viewModels_;
DataStore *dataStore_;
Wt::WPushButton *downloadButton_;
Wt::WPushButton *submitButton_;
Wt::WPushButton *bookmarkButton_;
Wt::WText *rateLabel_;
OJRatingSetWidget *rateSet_;

Wt::Signal<ProblemWidgetDialog> showDialog_;
};

class ProblemStatisticsDialog : public Wt::WDialog {
public:
ProblemStatisticsDialog(ViewModels *viewModels, DataStore *dataStore);
void login(Wt::Auth::Login& login);
void logout();
void setProblem(long long id);

private:
ViewModels *viewModels_;
DataStore *dataStore_;
};

class ProblemSubmissionDialog : public Wt::WDialog {
public:
ProblemSubmissionDialog(ViewModels *viewModels, DataStore *dataStore, Wt::Auth::Login *login);
void setProblem(long long id);
std::string code();
void setCode(std::string code);

private:
ViewModels *viewModels_;
DataStore *dataStore_;
OJCodeEditorWidget *codeEditor_;
Wt::Auth::Login *login_;
void saveSettings(OJCodeEditorSettings& settings);
};

#endif // PROBLEMWIDGET_H
