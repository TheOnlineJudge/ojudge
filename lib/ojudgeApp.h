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
#include <Wt/Auth/Login.h>
#include <Wt/WSignal.h>
#include <boost/program_options.hpp>
#include "viewmodels/ViewModels.h"
#include "datastore/DataStore.h"
#include "LoginWidget.h"

namespace xmlResources {
// Common
extern std::vector<const char *> form_templates_xml();

// English
extern std::vector<const char *> messages_xml();
extern std::vector<const char *> country_names_xml();
}

class Session;
class DBModel;

class ProblemWidget;
class AboutWidget;
class AdminWidget;
class ContactWidget;
class ContributeWidget;
class DashboardWidget;
class FactsWidget;
class LanguagesWidget;
class ProfileWidget;
class SponsorsWidget;
class TeamWidget;
class TutorialWidget;

class ojudgeApp : public Wt::WApplication {

public:
ojudgeApp(const Wt::WEnvironment& env, Session *session, ViewModels *viewModels, DBModel *dbmodel, DataStore *dataStore);
void authEvent();
ViewModels *getViewModels();

private:
bool googleAnalytics_ = false;
std::string googleAnalyticsId_;
Wt::WMenu *mainMenu_;
Wt::WMenu *mainFloatMenu_;
Wt::WStackedWidget *mainStack_;
ProblemWidget *problemWidget_;
AboutWidget *aboutWidget_;
AdminWidget *adminWidget_;
ContactWidget *contactWidget_;
ContributeWidget *contributeWidget_;
DashboardWidget *dashboardWidget_;
FactsWidget *factsWidget_;
LanguagesWidget *languagesWidget_;
ProfileWidget *profileWidget_;
SponsorsWidget *sponsorsWidget_;
TeamWidget *teamWidget_;
TutorialWidget *tutorialWidget_;
Wt::WMenuItem *dashboardMenu_;
Wt::WMenuItem *dashboardFloatMenu_;
Wt::WMenuItem *loginMenu_;
Wt::WMenuItem *loginFloatMenu_;
Wt::WMenuItem *profileMenu_;
Wt::WMenuItem *profileFloatMenu_;
Wt::WMenuItem *profilePopupAdmin_;
Wt::WMenuItem *profilePopupFloatAdmin_;
Wt::WNavigationBar *floatNavBar_;
Wt::Auth::AuthModel *authModel_;
LoginWidget *loginWidget_;

Wt::Signal<Wt::Auth::Login&>& loginSignal() {
	return loginSignal_;
}
Wt::Signal<>& logoutSignal() {
	return logoutSignal_;
}

Wt::Signal<Wt::Auth::Login&> loginSignal_;
Wt::Signal<> logoutSignal_;

void pathChanged(std::string newPath);
void switchNavbar(bool visible);
void showLoginBar();
void logout();

Session *session_;
ViewModels *viewModels_;
DBModel *dbmodel_;
DataStore *dataStore_;

};

#endif // OJUDGEAPP_H
