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

class DBModel;
class ViewModels;
class OJProblemViewerWidget;
class ProblemSidemenuWidget;

class ProblemWidget : public Wt::WContainerWidget {
public:
ProblemWidget(DBModel *dbmodel,ViewModels *viewModels);
void setProblem(long long id);

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
Wt::WText *pageTitle_;
OJProblemViewerWidget *problemViewer_;
ProblemSidemenuWidget *menuWidget_;
};

class ProblemSidemenuWidget : public Wt::WContainerWidget {
public:
ProblemSidemenuWidget(DBModel *dbmodel,ViewModels *viewModels);
void setProblem(long long id);

private:
DBModel *dbmodel_;
ViewModels *viewModels_;
Wt::WPushButton *downloadButton_;
};

#endif // PROBLEMWIDGET_H
