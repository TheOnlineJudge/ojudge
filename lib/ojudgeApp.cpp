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


	auto container = root()->addWidget(std::move(cpp14::make_unique<WContainerWidget>()));
	container->addStyleClass("stdwidth");
	container->addStyleClass("maxwidth");
	container->addStyleClass("container");

	auto vbox = container->setLayout(cpp14::make_unique<WVBoxLayout>());

	auto navigationBar = vbox->addWidget(std::move(cpp14::make_unique<WNavigationBar>()));
	navigationBar->setResponsive(true);
	navigationBar->addStyleClass("center");
	navigationBar->addStyleClass("header");
	navigationBar->addStyleClass("mynavbar");

	floatNavBar_ = vbox->addWidget(std::move(cpp14::make_unique<WNavigationBar>()));
	floatNavBar_->setResponsive(true);
	floatNavBar_->addStyleClass("center");
	floatNavBar_->addStyleClass("header");
	floatNavBar_->addStyleClass("navbar-fixed-top");
	floatNavBar_->addStyleClass("floatbar");
	floatNavBar_->hide();

	mainStack_ = vbox->addWidget(std::move(cpp14::make_unique<WStackedWidget>()));
	mainStack_->addStyleClass("center");
	mainStack_->addStyleClass("maincontent");

	auto footerWidget = vbox->addWidget(std::move(cpp14::make_unique<FooterWidget>(dbmodel_)));
	footerWidget->addStyleClass("center");
	footerWidget->addStyleClass("footer");

	auto logo = cpp14::make_unique<WImage>(dbmodel_->getSetting("siteLogo"));
	logo->setHeight(WLength(90));
	navigationBar->addWidget(std::move(logo));

	auto title = cpp14::make_unique<WText>(dbmodel_->getSetting("siteTitle"));
	title->addStyleClass("ojtitle");
	navigationBar->addWidget(std::move(title));

	auto floattitle = cpp14::make_unique<WText>(dbmodel_->getSetting("siteTitle"));
	floattitle->addStyleClass("ojfloattitle");
	floattitle->setHeight(WLength(50));
	floatNavBar_->addWidget(std::move(floattitle));

	mainMenu_ = navigationBar->addMenu(cpp14::make_unique<WMenu>(mainStack_),AlignmentFlag::Right);
	mainFloatMenu_ = floatNavBar_->addMenu(cpp14::make_unique<WMenu>(mainStack_),AlignmentFlag::Right);

	mainMenu_->setInternalPathEnabled("/");
	mainFloatMenu_->setInternalPathEnabled("/");
	auto homeMenu = mainMenu_->addItem(WString("Home"),std::move(cpp14::make_unique<HomeWidget>()));
	auto homeFloatMenu = mainFloatMenu_->addItem(WString("Home"));
	WLink homeFloatMenuLink(LinkType::InternalPath,"/home");
	homeFloatMenu->setLink(homeFloatMenuLink);

	auto problemsMenu = mainMenu_->addItem(WString("Problems"),std::move(cpp14::make_unique<ProblemsWidget>(viewModels_)));
	auto problemsFloatMenu = mainFloatMenu_->addItem(WString("Problems"));
	WLink problemsFloatMenuLink(LinkType::InternalPath,"/problems");
	problemsFloatMenu->setLink(problemsFloatMenuLink);

	auto submissionsMenu = mainMenu_->addItem(WString("Submissions"),std::move(cpp14::make_unique<SubmissionsWidget>()));
	auto submissionsFloatMenu = mainFloatMenu_->addItem(WString("Submissions"));
	WLink submissionsFloatMenuLink(LinkType::InternalPath,"/submissions");
	submissionsFloatMenu->setLink(submissionsFloatMenuLink);

	auto rankingsMenu = mainMenu_->addItem(WString("Rankings"),std::move(cpp14::make_unique<RankingsWidget>()));
	auto rankingsFloatMenu = mainFloatMenu_->addItem(WString("Rankings"));
	WLink rankingsFloatMenuLink(LinkType::InternalPath,"/rankings");
	rankingsFloatMenu->setLink(rankingsFloatMenuLink);

	auto contestsMenu = mainMenu_->addItem(WString("Contests"),std::move(cpp14::make_unique<ContestsWidget>()));
	auto contestsFloatMenu = mainFloatMenu_->addItem(WString("Contests"));
	WLink contestsFloatMenuLink(LinkType::InternalPath,"/contests");
	contestsFloatMenu->setLink(contestsFloatMenuLink);

	dashboardMenu_ = mainMenu_->addItem(WString("Dashboard"),std::move(cpp14::make_unique<DashboardWidget>()));
	dashboardFloatMenu_ = mainFloatMenu_->addItem(WString("Dashboard"));

	loginWidget_ = cpp14::make_unique<LoginWidget>(session_);

	loginMenu_ = mainMenu_->addItem(WString("Login"),std::move(loginWidget_));
	loginFloatMenu_ = mainFloatMenu_->addItem(WString("Login"));

	auto profilePopup = cpp14::make_unique<WPopupMenu>();
	auto profilePopupProfile = profilePopup->addItem(WString("Profile"));
	WLink profilePopupProfileLink(LinkType::InternalPath,"/profile");
	profilePopupProfile->setLink(profilePopupProfileLink);
	auto profilePopupAdmin = profilePopup->addItem(WString("Admin"));
	WLink profilePopupAdminLink(LinkType::InternalPath,"/admin");
	profilePopupAdmin->setLink(profilePopupAdminLink);
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
	profilePopupProfile->setLink(profilePopupProfileLink);
	profilePopupAdmin = profilePopup->addItem(WString("Admin"));
	profilePopupAdmin->setLink(profilePopupAdminLink);
	profilePopup->addSeparator();
	logoutItem = profilePopup->addItem(WString("Logout"));
	logoutItem->decorationStyle().setCursor(Cursor::PointingHand);
	logoutItem->clicked().connect(this,&ojudgeApp::logout);
	item = cpp14::make_unique<WMenuItem>("Profile");
	item->setInternalPathEnabled(false);
	item->setMenu(std::move(profilePopup));
	profileFloatMenu_ = mainFloatMenu_->addItem(std::move(item));

	problemWidget_ = mainStack_->addWidget(cpp14::make_unique<ProblemWidget>(dbmodel_,viewModels_));
	aboutWidget_ = mainStack_->addWidget(cpp14::make_unique<AboutWidget>());
	adminWidget_ = mainStack_->addWidget(cpp14::make_unique<AdminWidget>(session_,viewModels_,dbmodel_));
	contactWidget_ = mainStack_->addWidget(cpp14::make_unique<ContactWidget>());
	contributeWidget_ = mainStack_->addWidget(cpp14::make_unique<ContributeWidget>());
	factsWidget_ = mainStack_->addWidget(cpp14::make_unique<FactsWidget>());
	languagesWidget_ = mainStack_->addWidget(cpp14::make_unique<LanguagesWidget>());
	profileWidget_ = mainStack_->addWidget(cpp14::make_unique<ProfileWidget>(session_));
	sponsorsWidget_ = mainStack_->addWidget(cpp14::make_unique<SponsorsWidget>());
	teamWidget_ = mainStack_->addWidget(cpp14::make_unique<TeamWidget>());
	tutorialWidget_ = mainStack_->addWidget(cpp14::make_unique<TutorialWidget>());

	authEvent();
	session_->login().changed().connect(this, &ojudgeApp::authEvent);

	pathChanged(internalPath());
	internalPathChanged().connect(this,&ojudgeApp::pathChanged);

	navigationBar->setScrollVisibilityEnabled(true);
	navigationBar->scrollVisibilityChanged().connect(this,&ojudgeApp::switchNavbar);
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
		dashboardFloatMenu_->show();
		loginMenu_->hide();
		loginFloatMenu_->hide();
		profileMenu_->setText(session_->login().user().identity("loginname"));
		profileFloatMenu_->setText(session_->login().user().identity("loginname"));
		profileMenu_->show();
		profileFloatMenu_->show();
	} else {
		dashboardMenu_->hide();
		dashboardFloatMenu_->hide();
		profileMenu_->hide();
		profileFloatMenu_->hide();
		loginMenu_->show();
		loginFloatMenu_->show();
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

	if(mainFloatMenu_->currentItem() != NULL)
		mainFloatMenu_->currentItem()->renderSelected(false);

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
	} else {
		if(mainMenu_->currentItem() != NULL)
			mainMenu_->currentItem()->renderSelected(true);

		if(mainFloatMenu_->currentItem() != NULL)
			mainFloatMenu_->currentItem()->renderSelected(true);
	}

}

void ojudgeApp::switchNavbar(bool visible) {

	WAnimation anim(AnimationEffect::SlideInFromTop | AnimationEffect::Fade);

	if(visible) {
		floatNavBar_->animateHide(anim);
	} else {
		floatNavBar_->animateShow(anim);
	}
}

void ojudgeApp::logout() {
	session_->login().logout();
	setInternalPath("/home",true);
}
