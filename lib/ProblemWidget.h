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
#include <Wt/WText.h>
#include <Wt/WPushButton.h>
#include <Wt/WLink.h>
#include <Wt/WDialog.h>
#include "dbmodel/DBModel.h"

class ViewModels;
class OJProblemViewerWidget;
class OJCodeEditorWidget;
class ProblemDescriptionWidget;
class ProblemSidemenuWidget;
// Not for milestione 1.0.0 - class ProblemDiscussionDialog;
class ProblemStatisticsDialog;
class ProblemSubmissionDialog;

enum class ProblemWidgetDialog {
// Not for milestone 1.0.0	Discussion,
	Statistics,
	Submission
};

class ProblemWidget : public Wt::WContainerWidget {
public:
ProblemWidget(DBModel *dbmodel,ViewModels *viewModels);
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
// Not for milestone 1.0.0 - void showDiscussionDialog();
void showStatisticsDialog();
void showSubmissionDialog();
// Not for milestone 1.0.0 - void closeDiscussionDialog();
void closeStatisticsDialog();
void closeSubmissionDialog(Wt::DialogCode code);
};

class ProblemDescriptionWidget : public Wt::WContainerWidget {
public:
ProblemDescriptionWidget(DBModel *dbmodel,ViewModels *viewModels);
void setProblem(dbo::ptr<Problem> problemData);

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
OJProblemViewerWidget *problemViewer_;
};

class ProblemSidemenuWidget : public Wt::WContainerWidget {
public:
ProblemSidemenuWidget(DBModel *dbmodel,ViewModels *viewModels);
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

/* Not for milestone 1.0.0
class ProblemDiscussionDialog : public Wt::WDialog {
public:
ProblemDiscussionDialog(DBModel *dbmodel,ViewModels *viewModels);
void setProblem(dbo::ptr<Problem> problemData);

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
};
*/

class ProblemStatisticsDialog : public Wt::WDialog {
public:
ProblemStatisticsDialog(DBModel *dbmodel,ViewModels *viewModels);
void setProblem(dbo::ptr<Problem> problemData);

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
};

class ProblemSubmissionDialog : public Wt::WDialog {
public:
ProblemSubmissionDialog(DBModel *dbmodel,ViewModels *viewModels);
void setProblem(dbo::ptr<Problem> problemData);
std::string code();
void setCode(std::string code);

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
OJCodeEditorWidget *codeEditor_;
};

#endif // PROBLEMWIDGET_H
