/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <memory>
#include <string>
#include <sstream>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WImage.h>
#include <Wt/WText.h>
#include <Wt/WOverlayLoadingIndicator.h>
#include <Wt/WPushButton.h>
#include <Wt/WServer.h>
#include <Wt/WDialog.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WTemplate.h>
#include <Wt/WString.h>
#include <Wt/WAnimation.h>
#include <Wt/WStackedWidget.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/WAbstractItemModel.h>
#include "../version.h"
#include "dbmodel/DBModel.h"
#include "AboutWidget.h"
#include "AdminWidget.h"
#include "ContactWidget.h"
#include "ContestsWidget.h"
#include "ContributeWidget.h"
#include "DashboardWidget.h"
#include "FactsWidget.h"
#include "FooterWidget.h"
#include "HomeWidget.h"
#include "LanguagesWidget.h"
#include "LoginWidget.h"
#include "ProblemsWidget.h"
#include "ProfileWidget.h"
#include "RankingsWidget.h"
#include "SponsorsWidget.h"
#include "SubmissionsWidget.h"
#include "TeamWidget.h"
#include "TutorialWidget.h"
#include "ojudgeApp.h"

using namespace Wt;
namespace po = boost::program_options;

ojudgeApp::ojudgeApp(const WEnvironment& env, Session *session, ViewModels *viewModels, DBModel *dbmodel) : WApplication(env), session_(session), viewModels_(viewModels), dbmodel_(dbmodel) {

	enableUpdates(true);

	if(dbmodel_->getSetting("googleAnalytics") != "")
		require(std::string("https://www.googletagmanager.com/gtag/js?id=") + dbmodel_->getSetting("googleAnalytics"));

	setTitle(WString(dbmodel_->getSetting("siteTitle")));
	instance()->styleSheet().addRule(":root", std::string("--ojcolor: ") + dbmodel_->getSetting("siteColor"));
	useStyleSheet("css/ojudge.css");
	useStyleSheet("css/OJProblemViewer.css");
	setLoadingIndicator(cpp14::make_unique<WOverlayLoadingIndicator>());

	auto bootstrapTheme = std::make_shared<WBootstrapTheme>();
	bootstrapTheme->setVersion(BootstrapVersion::v3);
	bootstrapTheme->setResponsive(true);
	setTheme(bootstrapTheme);

	useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
	messageResourceBundle().useBuiltin(xmlResources::form_templates_xml1);
	messageResourceBundle().useBuiltin(xmlResources::messages_xml1);

	root()->setMargin(WLength(0));
	root()->setPadding(WLength(0));
	root()->addStyleClass("maxwidth");
	root()->addStyleClass("maxheight");

	auto container = root()->addNew<WContainerWidget>();

	auto navigationBar = container->addNew<WNavigationBar>();
	navigationBar->setResponsive(true);
	navigationBar->setTitle(dbmodel_->getSetting("siteTitle"), "/");
	navigationBar->addStyleClass("center");
	navigationBar->addStyleClass("header");
	navigationBar->addStyleClass("mynavbar");
	navigationBar->addStyleClass("navbar-fixed-top");

	mainStack_ = container->addNew<WStackedWidget>();
	mainStack_->addStyleClass("center");
	mainStack_->addStyleClass("maincontent");

	auto footerWidget = container->addNew<FooterWidget>(dbmodel_);
	footerWidget->addStyleClass("center");
	footerWidget->addStyleClass("footer");
	
	auto logo =	navigationBar->addWidget(cpp14::make_unique<WImage>(dbmodel_->getSetting("siteLogo")));
	logo->addStyleClass("homelogo");

	mainMenu_ = navigationBar->addMenu(cpp14::make_unique<WMenu>(mainStack_),AlignmentFlag::Right);
	mainMenu_->setInternalPathEnabled("/");
	
	auto homeMenu = mainMenu_->addItem(WString("Home"),std::move(cpp14::make_unique<HomeWidget>()));
	homeMenu->setLink(WLink(LinkType::InternalPath,"/home"));

	auto problemsMenu = mainMenu_->addItem(WString("Problems"),std::move(cpp14::make_unique<ProblemsWidget>(viewModels_)));
	problemsMenu->setLink(WLink(LinkType::InternalPath,"/problems"));

	auto submissionsMenu = mainMenu_->addItem(WString("Submissions"),std::move(cpp14::make_unique<SubmissionsWidget>()));
	submissionsMenu->setLink(WLink(LinkType::InternalPath,"/submissions"));

	auto rankingsMenu = mainMenu_->addItem(WString("Rankings"),std::move(cpp14::make_unique<RankingsWidget>()));
	rankingsMenu->setLink(WLink(LinkType::InternalPath,"/rankings"));

	auto contestsMenu = mainMenu_->addItem(WString("Contests"),std::move(cpp14::make_unique<ContestsWidget>()));
	contestsMenu->setLink(WLink(LinkType::InternalPath,"/contests"));

	dashboardMenu_ = mainMenu_->addItem(WString("Dashboard"),std::move(cpp14::make_unique<DashboardWidget>()));
	loginWidget_ = cpp14::make_unique<LoginWidget>(session_);
	loginMenu_ = mainMenu_->addItem(WString("Login"),std::move(loginWidget_));

	auto profilePopup = cpp14::make_unique<WPopupMenu>();
	auto profilePopupProfile = profilePopup->addItem(WString("Profile"));
	profilePopupProfile->setLink(WLink(LinkType::InternalPath,"/profile"));
	auto profilePopupAdmin = profilePopup->addItem(WString("Admin"));
	profilePopupAdmin->setLink(WLink(LinkType::InternalPath,"/admin"));
	profilePopup->addSeparator();

	auto logoutItem = profilePopup->addItem(WString("Logout"));
	logoutItem->decorationStyle().setCursor(Cursor::PointingHand);
	logoutItem->clicked().connect(this,&ojudgeApp::logout);

	auto item = cpp14::make_unique<WMenuItem>("Profile");
	item->setInternalPathEnabled(false);
	item->setMenu(std::move(profilePopup));
	profileMenu_ = mainMenu_->addItem(std::move(item));

	profilePopup = cpp14::make_unique<WPopupMenu>();
	profilePopupProfile = profilePopup->addItem(WString("Profile"));
	profilePopupProfile->setLink(WLink(LinkType::InternalPath,"/profile"));
	profilePopupAdmin = profilePopup->addItem(WString("Admin"));
	profilePopupAdmin->setLink(WLink(LinkType::InternalPath,"/admin"));
	profilePopup->addSeparator();

	logoutItem = profilePopup->addItem(WString("Logout"));
	logoutItem->decorationStyle().setCursor(Cursor::PointingHand);
	logoutItem->clicked().connect(this,&ojudgeApp::logout);
	item = cpp14::make_unique<WMenuItem>("Profile");
	item->setInternalPathEnabled(false);
	item->setMenu(std::move(profilePopup));

	problemWidget_ = mainStack_->addNew<ProblemWidget>(dbmodel_,viewModels_);
	aboutWidget_ = mainStack_->addNew<AboutWidget>();
	adminWidget_ = mainStack_->addNew<AdminWidget>(session_,viewModels_,dbmodel_);
	contactWidget_ = mainStack_->addNew<ContactWidget>();
	contributeWidget_ = mainStack_->addNew<ContributeWidget>();
	factsWidget_ = mainStack_->addNew<FactsWidget>();
	languagesWidget_ = mainStack_->addNew<LanguagesWidget>();
	profileWidget_ = mainStack_->addNew<ProfileWidget>(session_);
	sponsorsWidget_ = mainStack_->addNew<SponsorsWidget>();
	teamWidget_ = mainStack_->addNew<TeamWidget>();
	tutorialWidget_ = mainStack_->addNew<TutorialWidget>();

	authEvent();
	session_->login().changed().connect(this, &ojudgeApp::authEvent);

	pathChanged(internalPath());
	internalPathChanged().connect(this,&ojudgeApp::pathChanged);

	navigationBar->setScrollVisibilityEnabled(true);
}

ViewModels *ojudgeApp::getViewModels() {
	return viewModels_;
}

void ojudgeApp::authEvent() {
	if(session_->login().loggedIn()) {
//              log("notice") << "User " << session_.login().user().id() << " logged in.";
//              Dbo::Transaction t(*session_);
//              dbo::ptr<User> user = session_->user();
//              log("notice") << "(Favourite pet: " << user->favouritePet << ")";
		dashboardMenu_->show();
		loginMenu_->hide();
		profileMenu_->setText(session_->login().user().identity("loginname"));
		profileMenu_->show();
	} else {
		dashboardMenu_->hide();
		profileMenu_->hide();
		loginMenu_->show();
//              log("notice") << "User logged out.";
	}
}

void ojudgeApp::pathChanged(std::string newPath) {

	if(newPath == "/profile" || newPath == "/dashboard") {
		if(!session_->login().loggedIn()) {
			setInternalPath("/login",true);
			return;
		}
	}

	if(dbmodel_->getSetting("googleAnalytics") != "") {
		doJavaScript(   "window.dataLayer = window.dataLayer || [];"
		                "function gtag(){dataLayer.push(arguments);}"
		                "gtag('js', new Date());"
		                "gtag('config', '" + dbmodel_->getSetting("googleAnalytics") + "', {"
		                "'page_path': '" + newPath + "'"
		                "});");
	}

	if(mainMenu_->currentItem() != NULL)
		mainMenu_->currentItem()->renderSelected(false);

	if(newPath.substr(0,9)=="/problem/") {
		problemWidget_->setProblem(std::stoi(newPath.substr(9)));
		mainStack_->setCurrentWidget(problemWidget_);
	} else if(newPath=="/about") {
		mainStack_->setCurrentWidget(aboutWidget_);
	} else if (newPath=="/profile") {
		mainStack_->setCurrentWidget(profileWidget_);
	} else if (newPath=="/admin") {
		mainStack_->setCurrentWidget(adminWidget_);
	} else if (newPath=="/team") {
		mainStack_->setCurrentWidget(teamWidget_);
	} else if (newPath=="/facts") {
		mainStack_->setCurrentWidget(factsWidget_);
	} else if (newPath=="/languages") {
		mainStack_->setCurrentWidget(languagesWidget_);
	} else if (newPath=="/sponsors") {
		mainStack_->setCurrentWidget(sponsorsWidget_);
	} else if (newPath=="/tutorial") {
		mainStack_->setCurrentWidget(tutorialWidget_);
	} else if (newPath=="/contact") {
		mainStack_->setCurrentWidget(contactWidget_);
	} else if (newPath=="/contribute") {
		mainStack_->setCurrentWidget(contributeWidget_);
	} else if(mainMenu_->currentItem() != NULL)
			mainMenu_->currentItem()->renderSelected(true);
}

void ojudgeApp::logout() {
	session_->login().logout();
	setInternalPath("/home",true);
}
