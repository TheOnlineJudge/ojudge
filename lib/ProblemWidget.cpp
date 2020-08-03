/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/Utils.h>
#include "ProblemWidget.h"
#include "PdfResource.h"
#include "widgets/OJProblemViewerWidget.h"
#include "datastore/UserStore.h"

using namespace Wt;

ProblemWidget::ProblemWidget(ViewModels *viewModels, DataStore *dataStore) : viewModels_(viewModels), dataStore_(dataStore) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	pageTitle_ = mainLayout->addWidget(cpp14::make_unique<WText>("Problem"),0);
	pageTitle_->addStyleClass("oj-pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

	auto menuLayout = mainWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
	menuLayout->setContentsMargins(0,0,0,0);

	descriptionWidget_ = menuLayout->addWidget(cpp14::make_unique<ProblemDescriptionWidget>(viewModels_,dataStore_),1);
	loginSignal().connect(descriptionWidget_,&ProblemDescriptionWidget::login);
	logoutSignal().connect(descriptionWidget_,&ProblemDescriptionWidget::logout);

	menuWidget_ = menuLayout->addWidget(cpp14::make_unique<ProblemSidemenuWidget>(viewModels_,dataStore_),0);
	loginSignal().connect(menuWidget_,&ProblemSidemenuWidget::login);
	logoutSignal().connect(menuWidget_,&ProblemSidemenuWidget::logout);
	menuWidget_->setWidth("35%");
	menuWidget_->setMinimumSize(200, "100%");

	statisticsDialog_ = addChild(cpp14::make_unique<ProblemStatisticsDialog>(viewModels_,dataStore_));
	loginSignal().connect(statisticsDialog_,&ProblemStatisticsDialog::login);
	logoutSignal().connect(statisticsDialog_,&ProblemStatisticsDialog::logout);

	menuWidget_->showDialog().connect( [=] (ProblemWidgetDialog dialog) {
		switch(dialog) {
		case ProblemWidgetDialog::Submission:
			showSubmissionDialog();
			break;
		case ProblemWidgetDialog::Statistics:
			showStatisticsDialog();
			break;
		}
	});

}

void ProblemWidget::login(Auth::Login& login) {
	login_ = &login;
	loginSignal().emit(login);
}

void ProblemWidget::logout() {
	logoutSignal().emit();
}

void ProblemWidget::setProblem(long long id) {

	problemData_ = dataStore_->getProblemStore()->getProblemById(id);

	if(!problemData_) {
		pageTitle_->setText("Problem not found");
		return;
	}

	pageTitle_->setText(std::to_string(id) + " - " + problemData_.value().title);

	descriptionWidget_->setProblem(id);
	menuWidget_->setProblem(id);
}

void ProblemWidget::showSubmissionDialog() {

	submissionDialog_ = addChild(cpp14::make_unique<ProblemSubmissionDialog>(viewModels_,dataStore_,login_));
	//submissionDialog_->resize("100%", "100%");
	submissionDialog_->setProblem(problemData_.value().id);

	submissionDialog_->finished().connect(this,&ProblemWidget::closeSubmissionDialog);
	submissionDialog_->show();
}

void ProblemWidget::closeSubmissionDialog(DialogCode code) {

	switch(code) {
	case DialogCode::Accepted:
		break;
	case DialogCode::Rejected:
		WStringStream strm;

		strm << "sessionStorage.setItem('OJdraft" << problemData_.value().id << "',";
		strm << "'" << Utils::base64Encode(Utils::urlEncode(submissionDialog_->code()),false) << "');";

		doJavaScript(strm.str());
		break;
	}

	removeChild(submissionDialog_);
}

void ProblemWidget::showStatisticsDialog() {

	statisticsDialog_ = addChild(cpp14::make_unique<ProblemStatisticsDialog>(viewModels_,dataStore_));
	
	statisticsDialog_->setProblem(problemData_.value().id);

	statisticsDialog_->finished().connect(this,&ProblemWidget::closeStatisticsDialog);
	statisticsDialog_->show();
}

void ProblemWidget::closeStatisticsDialog() {

	removeChild(statisticsDialog_);
}

ProblemDescriptionWidget::ProblemDescriptionWidget(ViewModels *viewModels, DataStore *dataStore) : viewModels_(viewModels), dataStore_(dataStore) {

	problemViewer_ = addWidget(cpp14::make_unique<OJProblemViewerWidget>());
}

void ProblemDescriptionWidget::login(Auth::Login& login) {

}

void ProblemDescriptionWidget::logout() {

}

void ProblemDescriptionWidget::setProblem(long long id) {

	problemViewer_->setContent(dataStore_->getProblemStore()->getHtmlDescription(id));
}

ProblemSidemenuWidget::ProblemSidemenuWidget(ViewModels *viewModels, DataStore *dataStore) : viewModels_(viewModels), dataStore_(dataStore) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());

	submitButton_ = mainLayout->addWidget(cpp14::make_unique<WPushButton>("Submit solution"),0);
	submitButton_->clicked().connect( [=] {
		showDialog_.emit(ProblemWidgetDialog::Submission);
	});
	submitButton_->disable();
	submitButton_->setToolTip("You have to log in to submit a solution.");

	auto downloadBookmarkLayout = mainLayout->addLayout(cpp14::make_unique<WHBoxLayout>(),0);
	downloadBookmarkLayout->setContentsMargins(0,0,0,0);

	downloadButton_ = downloadBookmarkLayout->addWidget(cpp14::make_unique<WPushButton>("Download PDF"),0);
	downloadButton_->setLink(WLink(std::make_shared<PdfResource>(dataStore_->getProblemStore())));
	downloadButton_->setIcon(WLink("images/pdf.svg"));

	bookmarkButton_ = downloadBookmarkLayout->addWidget(cpp14::make_unique<WPushButton>("Bookmark problem"),0);
	bookmarkButton_->setCheckable(true);
	bookmarkButton_->setIcon(WLink("images/bookmark.svg"));
	bookmarkButton_->disable();
	bookmarkButton_->setToolTip("You have to log in to bookmark a problem.");

	// Placeholder for problem info (CPU limit, RAM limit, etc.)
	auto infoFrame = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0);
	infoFrame->setHeight(200);
	infoFrame->setContentAlignment(AlignmentFlag::Center);
	auto infoText = infoFrame->addWidget(cpp14::make_unique<WText>("Problem info<br/>CPU limit<br/>RAM limit<br/>Author<br/>Added date<br/>etc."));
	infoText->setTextAlignment(AlignmentFlag::Center);

	// Placeholder, to be substituted with an actual widget
	auto statsFrame = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0);
	statsFrame->setHeight(250);
	statsFrame->decorationStyle().setBorder(WBorder(BorderStyle::Solid,1));
	statsFrame->decorationStyle().setCursor(Cursor::PointingHand);
	statsFrame->setContentAlignment(AlignmentFlag::Center);
	auto statsText = statsFrame->addWidget(cpp14::make_unique<WText>("Brief statistics<br/>Clickable for more"));
	statsText->setTextAlignment(AlignmentFlag::Center);
	statsFrame->clicked().connect( [=] {
		showDialog_.emit(ProblemWidgetDialog::Statistics);
	});

	rateLabel_ = mainLayout->addWidget(cpp14::make_unique<WText>("Rate this problem"));
	rateLabel_->setTextAlignment(AlignmentFlag::Center);

	auto rateLayout = mainLayout->addLayout(cpp14::make_unique<WHBoxLayout>());
	rateLayout->setContentsMargins(0,0,0,0);
	rateLayout->addStretch(1);
	rateSet_ = rateLayout->addWidget(cpp14::make_unique<OJRatingSetWidget>(),0);
	rateSet_->disable();
	rateSet_->setToolTip("You have to log in to rate a problem.");
	rateLayout->addStretch(1);

	mainLayout->addStretch(1);
}

void ProblemSidemenuWidget::login(Auth::Login& login) {

	submitButton_->enable();
	submitButton_->setToolTip("");
	bookmarkButton_->enable();
	bookmarkButton_->setToolTip("");
	rateSet_->enable();
	rateSet_->setToolTip("");
}

void ProblemSidemenuWidget::logout() {

	submitButton_->disable();
	submitButton_->setToolTip("You have to log in to submit a solution.");
	bookmarkButton_->disable();
	bookmarkButton_->setToolTip("You have to log in to bookmark a problem.");
	rateSet_->disable();
	rateSet_->setToolTip("You have to log in to rate a problem.");
}

void ProblemSidemenuWidget::setProblem(long long id) {

	PdfResource *tmpPdfRes = (PdfResource*)downloadButton_->link().resource().get();
	tmpPdfRes->setProblem(id);
}

ProblemStatisticsDialog::ProblemStatisticsDialog(ViewModels *viewModels, DataStore *dataStore) : viewModels_(viewModels), dataStore_(dataStore) {
	setWindowTitle("Statistics dialog");
	setClosable(true);

	contents()->addWidget(cpp14::make_unique<WText>("Statistics stuff"));
}

void ProblemStatisticsDialog::login(Auth::Login& login) {

}

void ProblemStatisticsDialog::logout() {

}

void ProblemStatisticsDialog::setProblem(long long id) {

}

ProblemSubmissionDialog::ProblemSubmissionDialog(ViewModels *viewModels, DataStore *dataStore, Auth::Login *login)
	: viewModels_(viewModels), dataStore_(dataStore), login_(login) {
	setWindowTitle("Submit");
	setClosable(true);
	setId("editor_box");
	resize("80%", "100%");
	setMinimumSize("50%", "50%");
	setLayoutSizeAware(true);

	auto hpanel  = contents()->setLayout(cpp14::make_unique<Wt::WHBoxLayout>());
	auto left_buttons = hpanel->addWidget(cpp14::make_unique<Wt::WContainerWidget>());
	auto left_buttons_layout = left_buttons->setLayout(cpp14::make_unique<Wt::WVBoxLayout>());
	left_buttons->setWidth(200);

	WStringStream strm;
	strm << left_buttons->jsRef() << ".style.resize='horizontal';";
	doJavaScript(strm.str());

	WPushButton *submit = left_buttons_layout->addWidget(cpp14::make_unique<WPushButton>("Submit"));
	submit->clicked().connect(this,&WDialog::accept);
	
	WContainerWidget *submitedCard = left_buttons_layout->addWidget(cpp14::make_unique<WContainerWidget>());
	auto submitedCardLayout = submitedCard->setLayout(cpp14::make_unique<Wt::WVBoxLayout>());
	submitedCard->addStyleClass("oj-code-card");
	submitedCardLayout->addWidget(cpp14::make_unique<WText>("DRAFT"));
	auto saved_at = submitedCardLayout->addWidget(cpp14::make_unique<WText>(""));

	// This is only an example, we should recover from SumbssionStore the last N submissions and
	// make a code-card with this format for each sumbmission
	WContainerWidget *example = left_buttons_layout->addWidget(cpp14::make_unique<WContainerWidget>());
	example->addStyleClass("oj-code-card");
	auto exampleLayout = example->setLayout(cpp14::make_unique<Wt::WVBoxLayout>());
	exampleLayout->addWidget(cpp14::make_unique<WText>("WA"));
	exampleLayout->addWidget(cpp14::make_unique<WText>("20/07/2020"));
	
	left_buttons_layout->addStretch(1);

	LanguageStore *languageStore = dataStore_->getLanguageStore();
	std::map<std::string, std::string> languages;
	for( auto &i : languageStore->getLanguages()){
		if(i.second.aceStyle.has_value())
			languages[i.second.name] = i.second.aceStyle.value(); 
	}

	codeEditor_ = hpanel->addWidget(cpp14::make_unique<OJCodeEditorWidget>(languages));
	codeEditor_->resize("100%","100%");
	codeEditor_->settingsChanged().connect(this,&ProblemSubmissionDialog::saveSettings);

	OJCodeEditorSettings settings;

	UserStore *userStore = dataStore_->getUserStore();
	settings.fontsize = cpp17::any_cast<int>(userStore->getUserSetting(login_->user(),UserSettingType::EditorFontsize));
	settings.indent = cpp17::any_cast<int>(userStore->getUserSetting(login_->user(),UserSettingType::EditorIndent));
	settings.wrap = cpp17::any_cast<bool>(userStore->getUserSetting(login_->user(),UserSettingType::EditorWrap));
	settings.theme = cpp17::any_cast<std::string>(userStore->getUserSetting(login_->user(),UserSettingType::EditorTheme));

	doJavaScript("setTimeout(() => {\
		editor_box = document.getElementById('editor_box').children[0];\
				editor_box.style.height = '100%';\
				editor_box.style.width = '100%';\
		}, 250 );");
}

void ProblemSubmissionDialog::setProblem(long long id) {

	ProblemData problemData = dataStore_->getProblemStore()->getProblemById(id).value();

	setWindowTitle(std::to_string(id)  + " - " + problemData.title);

	codeEditor_->loadCodeFromSession("OJdraft" + std::to_string(id));
}

std::string ProblemSubmissionDialog::code() {
	return codeEditor_->code();
}

void ProblemSubmissionDialog::setCode(std::string code) {
	codeEditor_->setCode(code);
}

void ProblemSubmissionDialog::saveSettings(OJCodeEditorSettings& settings) {

	UserStore *userStore = dataStore_->getUserStore();

	userStore->setUserSetting(login_->user(),UserSettingType::EditorFontsize,settings.fontsize);
	userStore->setUserSetting(login_->user(),UserSettingType::EditorIndent,settings.indent);
	userStore->setUserSetting(login_->user(),UserSettingType::EditorWrap,settings.wrap);
	userStore->setUserSetting(login_->user(),UserSettingType::EditorTheme,settings.theme);
	userStore->setUserSetting(login_->user(),UserSettingType::EditorStyle,settings.style);
}
