/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef OJUDGEAPP_H
#define OJUDGEAPP_H

#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>
#include <Wt/WMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WMenuItem.h>
#include <Wt/WNavigationBar.h>
#include <Wt/Auth/AuthModel.h>
#include <Wt/WAbstractItemModel.h>
#include <boost/program_options.hpp>
#include "viewmodels/ViewModels.h"
#include "LoginWidget.h"
#include "ProblemWidget.h"

namespace xmlResources {
// Common
extern const char *form_templates_xml1;

// English
extern const char *messages_xml1;
}

class Session;
class DBModel;

class ojudgeApp : public Wt::WApplication {

public:
ojudgeApp(const Wt::WEnvironment& env, Session *session, ViewModels *viewModels, DBModel *dbmodel);
void authEvent();
ViewModels *getViewModels();

private:
Wt::WMenu *mainMenu_;
Wt::WStackedWidget *mainStack_;
ProblemWidget *problemWidget_;
Wt::WContainerWidget *aboutWidget_;
Wt::WContainerWidget *adminWidget_;
Wt::WContainerWidget *contactWidget_;
Wt::WContainerWidget *contributeWidget_;
Wt::WContainerWidget *factsWidget_;
Wt::WContainerWidget *languagesWidget_;
Wt::WContainerWidget *profileWidget_;
Wt::WContainerWidget *sponsorsWidget_;
Wt::WContainerWidget *teamWidget_;
Wt::WContainerWidget *tutorialWidget_;
Wt::WMenuItem *dashboardMenu_;
Wt::WMenuItem *loginMenu_;
Wt::WMenuItem *profileMenu_;
Wt::Auth::AuthModel *authModel_;
std::unique_ptr<LoginWidget> loginWidget_;

void pathChanged(std::string newPath);
void logout();

Session *session_;
ViewModels *viewModels_;
DBModel *dbmodel_;

};

#endif // OJUDGEAPP_H
