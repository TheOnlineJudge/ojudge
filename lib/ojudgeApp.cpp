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
#include "datastore/SettingStore.h"
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
#include "ProblemWidget.h"
#include "ProfileWidget.h"
#include "RankingsWidget.h"
#include "SponsorsWidget.h"
#include "SubmissionsWidget.h"
#include "TeamWidget.h"
#include "TutorialWidget.h"
#include "ojudgeApp.h"

using namespace Wt;
namespace po = boost::program_options;

ojudgeApp::ojudgeApp(const WEnvironment& env, Session *session, ViewModels *viewModels, DBModel *dbmodel, DataStore *dataStore) : WApplication(env), session_(session), viewModels_(viewModels), dbmodel_(dbmodel), dataStore_(dataStore) {

	enableUpdates(true);

	if(dataStore_->getSettingStore()->getSetting("googleanalytics") != "") {
		googleAnalytics_ = true;
		googleAnalyticsId_ = dataStore_->getSettingStore()->getSetting("googleanalytics");
		require(std::string("https://www.googletagmanager.com/gtag/js?id=") + googleAnalyticsId_ );
	}


	setTitle(WString(dataStore_->getSettingStore()->getSetting("sitetitle")));
	instance()->styleSheet().addRule(":root", std::string("--ojcolor: ") + dataStore_->getSettingStore()->getSetting("sitecolor"));
	useStyleSheet("css/ojudge.css");
	useStyleSheet("css/oj-problem-viewer.css");
	setLoadingIndicator(cpp14::make_unique<WOverlayLoadingIndicator>());

	auto bootstrapTheme = std::make_shared<WBootstrapTheme>();
	bootstrapTheme->setVersion(BootstrapVersion::v3);
	bootstrapTheme->setResponsive(true);
	setTheme(bootstrapTheme);

	useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
	messageResourceBundle().useBuiltin(xmlResources::form_templates_xml()[0]);
	messageResourceBundle().useBuiltin(xmlResources::messages_xml()[0]);
	messageResourceBundle().useBuiltin(xmlResources::country_names_xml()[0]);

	root()->setMargin(WLength(0));
	root()->setPadding(WLength(0));
	root()->addStyleClass("oj-maxwidth");
	root()->addStyleClass("oj-maxheight");

	auto container = root()->addWidget(std::move(cpp14::make_unique<WContainerWidget>()));
	container->addStyleClass("oj-stdwidth");
	container->addStyleClass("oj-maxwidth");
	container->addStyleClass("container");

	auto vbox = container->setLayout(cpp14::make_unique<WVBoxLayout>());
	vbox->setContentsMargins(0,0,0,0);

	auto navigationBar = vbox->addWidget(std::move(cpp14::make_unique<WNavigationBar>()));
	navigationBar->setResponsive(true);
	navigationBar->addStyleClass("center");
	navigationBar->addStyleClass("oj-header");
	navigationBar->addStyleClass("oj-navbar");

	floatNavBar_ = vbox->addWidget(std::move(cpp14::make_unique<WNavigationBar>()));
	floatNavBar_->setResponsive(true);
	floatNavBar_->addStyleClass("center");
	floatNavBar_->addStyleClass("oj-header");
	floatNavBar_->addStyleClass("navbar-fixed-top");
	floatNavBar_->addStyleClass("oj-floatbar");
	floatNavBar_->hide();

	auto stackContainer  = vbox->addWidget(std::move(cpp14::make_unique<WContainerWidget>()));
	stackContainer->setMargin(6,Side::Top);
	mainStack_ = stackContainer->addWidget(std::move(cpp14::make_unique<WStackedWidget>()));
	mainStack_->addStyleClass("center");
	mainStack_->addStyleClass("oj-maincontent");

	auto footerWidget = stackContainer->addWidget(std::move(cpp14::make_unique<FooterWidget>(dataStore_->getSettingStore())));
	loginSignal().connect(footerWidget,&FooterWidget::login);
	logoutSignal().connect(footerWidget,&FooterWidget::logout);
	footerWidget->addStyleClass("center");

	auto logo = cpp14::make_unique<WImage>(dataStore_->getSettingStore()->getSetting("sitelogo"));
	logo->setHeight(WLength(90));
	navigationBar->addWidget(std::move(logo));

	auto title = cpp14::make_unique<WText>(dataStore_->getSettingStore()->getSetting("sitetitle"));
	title->addStyleClass("oj-title");
	navigationBar->addWidget(std::move(title));

	auto floattitle = cpp14::make_unique<WText>(dataStore_->getSettingStore()->getSetting("sitetitle"));
	floattitle->addStyleClass("oj-float-title");
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

	auto problemsWidget = cpp14::make_unique<ProblemsWidget>(viewModels_);
	loginSignal().connect(problemsWidget.get(),&ProblemsWidget::login);
	logoutSignal().connect(problemsWidget.get(),&ProblemsWidget::logout);
	auto problemsMenu = mainMenu_->addItem(WString("Problems"),std::move(problemsWidget));
	auto problemsFloatMenu = mainFloatMenu_->addItem(WString("Problems"));
	WLink problemsFloatMenuLink(LinkType::InternalPath,"/problems");
	problemsFloatMenu->setLink(problemsFloatMenuLink);

	auto submissionsWidget = cpp14::make_unique<SubmissionsWidget>();
	loginSignal().connect(submissionsWidget.get(),&SubmissionsWidget::login);
	logoutSignal().connect(submissionsWidget.get(),&SubmissionsWidget::logout);
	auto submissionsMenu = mainMenu_->addItem(WString("Submissions"),std::move(submissionsWidget));
	auto submissionsFloatMenu = mainFloatMenu_->addItem(WString("Submissions"));
	WLink submissionsFloatMenuLink(LinkType::InternalPath,"/submissions");
	submissionsFloatMenu->setLink(submissionsFloatMenuLink);

	auto rankingsWidget = cpp14::make_unique<RankingsWidget>();
	loginSignal().connect(rankingsWidget.get(),&RankingsWidget::login);
	logoutSignal().connect(rankingsWidget.get(),&RankingsWidget::logout);
	auto rankingsMenu = mainMenu_->addItem(WString("Rankings"),std::move(rankingsWidget));
	auto rankingsFloatMenu = mainFloatMenu_->addItem(WString("Rankings"));
	WLink rankingsFloatMenuLink(LinkType::InternalPath,"/rankings");
	rankingsFloatMenu->setLink(rankingsFloatMenuLink);

	auto contestsWidget = cpp14::make_unique<ContestsWidget>();
	loginSignal().connect(contestsWidget.get(),&ContestsWidget::login);
	logoutSignal().connect(contestsWidget.get(),&ContestsWidget::logout);
	auto contestsMenu = mainMenu_->addItem(WString("Contests"),std::move(contestsWidget));
	auto contestsFloatMenu = mainFloatMenu_->addItem(WString("Contests"));
	WLink contestsFloatMenuLink(LinkType::InternalPath,"/contests");
	contestsFloatMenu->setLink(contestsFloatMenuLink);

	auto dashboardWidget = cpp14::make_unique<DashboardWidget>();
	dashboardWidget_ = dashboardWidget.get();
	loginSignal().connect(dashboardWidget_,&DashboardWidget::login);
	logoutSignal().connect(dashboardWidget_,&DashboardWidget::logout);
	dashboardMenu_ = mainMenu_->addItem(WString("Dashboard"),std::move(dashboardWidget));
	dashboardFloatMenu_ = mainFloatMenu_->addItem(WString("Dashboard"));

	loginWidget_ = vbox->addWidget(std::move(cpp14::make_unique<LoginWidget>(session_)));
	loginSignal().connect(loginWidget_,&LoginWidget::login);
	logoutSignal().connect(loginWidget_,&LoginWidget::logout);
	loginWidget_->addStyleClass("oj-login-widget");
	loginWidget_->hide();

	loginMenu_ = mainMenu_->addItem(WString("Login"));
	loginMenu_->setInternalPathEnabled(false);
	loginMenu_->setSelectable(false);
	loginMenu_->clicked().connect(this,&ojudgeApp::showLoginBar);
	loginFloatMenu_ = mainFloatMenu_->addItem(WString("Login"));
	loginFloatMenu_->setInternalPathEnabled(false);
	loginFloatMenu_->setSelectable(false);
	loginFloatMenu_->clicked().connect(this,&ojudgeApp::showLoginBar);

	auto profilePopup = cpp14::make_unique<WPopupMenu>();
	auto profilePopupProfile = profilePopup->addItem(WString("Profile"));
	WLink profilePopupProfileLink(LinkType::InternalPath,"/profile");
	profilePopupProfile->setLink(profilePopupProfileLink);
	profilePopupAdmin_ = profilePopup->addItem(WString("Admin"));
	WLink profilePopupAdminLink(LinkType::InternalPath,"/admin");
	profilePopupAdmin_->setLink(profilePopupAdminLink);
	profilePopupAdmin_->hide();
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
	profilePopupFloatAdmin_ = profilePopup->addItem(WString("Admin"));
	profilePopupFloatAdmin_->setLink(profilePopupAdminLink);
	profilePopupFloatAdmin_->hide();
	profilePopup->addSeparator();
	logoutItem = profilePopup->addItem(WString("Logout"));
	logoutItem->decorationStyle().setCursor(Cursor::PointingHand);
	logoutItem->clicked().connect(this,&ojudgeApp::logout);
	item = cpp14::make_unique<WMenuItem>("Profile");
	item->setInternalPathEnabled(false);
	item->setMenu(std::move(profilePopup));
	profileFloatMenu_ = mainFloatMenu_->addItem(std::move(item));

	problemWidget_ = mainStack_->addWidget(cpp14::make_unique<ProblemWidget>(viewModels_,dataStore_));
	loginSignal().connect(problemWidget_,&ProblemWidget::login);
	logoutSignal().connect(problemWidget_,&ProblemWidget::logout);

	aboutWidget_ = mainStack_->addWidget(cpp14::make_unique<AboutWidget>());
	loginSignal().connect(aboutWidget_,&AboutWidget::login);
	logoutSignal().connect(aboutWidget_,&AboutWidget::logout);

	adminWidget_ = mainStack_->addWidget(cpp14::make_unique<AdminWidget>(session_,viewModels_,dbmodel_,dataStore_));
	loginSignal().connect(adminWidget_,&AdminWidget::login);
	logoutSignal().connect(adminWidget_,&AdminWidget::logout);

	contactWidget_ = mainStack_->addWidget(cpp14::make_unique<ContactWidget>());
	loginSignal().connect(contactWidget_,&ContactWidget::login);
	logoutSignal().connect(contactWidget_,&ContactWidget::logout);

	contributeWidget_ = mainStack_->addWidget(cpp14::make_unique<ContributeWidget>());
	loginSignal().connect(contributeWidget_,&ContributeWidget::login);
	logoutSignal().connect(contributeWidget_,&ContributeWidget::logout);

	factsWidget_ = mainStack_->addWidget(cpp14::make_unique<FactsWidget>());
	loginSignal().connect(factsWidget_,&FactsWidget::login);
	logoutSignal().connect(factsWidget_,&FactsWidget::logout);

	languagesWidget_ = mainStack_->addWidget(cpp14::make_unique<LanguagesWidget>());
	loginSignal().connect(languagesWidget_,&LanguagesWidget::login);
	logoutSignal().connect(languagesWidget_,&LanguagesWidget::logout);

	profileWidget_ = mainStack_->addWidget(cpp14::make_unique<ProfileWidget>(dataStore_,viewModels_->getCountryModel(),loginWidget_->authWidget()));
	loginSignal().connect(profileWidget_,&ProfileWidget::login);
	logoutSignal().connect(profileWidget_,&ProfileWidget::logout);

	sponsorsWidget_ = mainStack_->addWidget(cpp14::make_unique<SponsorsWidget>());
	loginSignal().connect(sponsorsWidget_,&SponsorsWidget::login);
	logoutSignal().connect(sponsorsWidget_,&SponsorsWidget::logout);

	teamWidget_ = mainStack_->addWidget(cpp14::make_unique<TeamWidget>());
	loginSignal().connect(teamWidget_,&TeamWidget::login);
	logoutSignal().connect(teamWidget_,&TeamWidget::logout);

	tutorialWidget_ = mainStack_->addWidget(cpp14::make_unique<TutorialWidget>());
	loginSignal().connect(tutorialWidget_,&TutorialWidget::login);
	logoutSignal().connect(tutorialWidget_,&TutorialWidget::logout);

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
		dashboardMenu_->show();
		dashboardFloatMenu_->show();
		loginMenu_->hide();
		loginFloatMenu_->hide();
		profileMenu_->setText(session_->login().user().identity("loginname"));
		profileFloatMenu_->setText(session_->login().user().identity("loginname"));
		profileMenu_->show();
		profileFloatMenu_->show();

		dbo::Transaction transaction = dbmodel_->startTransaction();
		dbo::ptr<User> userData = session_->user(session_->login().user());
		if(userData->role == Role::Admin) {
			profilePopupAdmin_->show();
			profilePopupFloatAdmin_->show();
		}

		loginSignal().emit(session_->login());
	} else {
		dashboardMenu_->hide();
		dashboardFloatMenu_->hide();
		profilePopupAdmin_->hide();
		profilePopupFloatAdmin_->hide();
		profileMenu_->hide();
		profileFloatMenu_->hide();
		loginMenu_->show();
		loginFloatMenu_->show();

		logoutSignal().emit();
	}
}

void ojudgeApp::pathChanged(std::string newPath) {

	if(googleAnalytics_) {
		doJavaScript(   "window.dataLayer = window.dataLayer || [];"
		                "function gtag(){dataLayer.push(arguments);}"
		                "gtag('js', new Date());"
		                "gtag('config', '" + googleAnalyticsId_ + "', {"
		                "'page_path': '" + newPath + "'"
		                "});");
	}

	if(mainMenu_->currentItem() != NULL)
		mainMenu_->currentItem()->renderSelected(false);

	if(mainFloatMenu_->currentItem() != NULL)
		mainFloatMenu_->currentItem()->renderSelected(false);

	if(internalPathMatches("/problem/")) {
		problemWidget_->setProblem(std::stoi(internalPathNextPart("/problem/")));
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

void ojudgeApp::showLoginBar() {

	WAnimation anim(AnimationEffect::SlideInFromTop | AnimationEffect::Fade);

	loginWidget_->animateShow(anim);
	doJavaScript("setTimeout(() => { \
		document.getElementById('username_field').control.focus() \
		}," + std::to_string(anim.duration()) + ");");
	

}

void ojudgeApp::logout() {
	session_->login().logout();
	setInternalPath("/home",true);
}
