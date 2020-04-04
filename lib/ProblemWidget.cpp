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
#include "widgets/OJRatingSetWidget.h"

using namespace Wt;

ProblemWidget::ProblemWidget(DBModel *dbmodel, ViewModels *viewModels, Session *session) : dbmodel_(dbmodel),viewModels_(viewModels),session_(session) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	pageTitle_ = mainLayout->addWidget(cpp14::make_unique<WText>("Problem"),0);
	pageTitle_->addStyleClass("oj-pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

	auto menuLayout = mainWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
	menuLayout->setContentsMargins(0,0,0,0);

	descriptionWidget_ = menuLayout->addWidget(cpp14::make_unique<ProblemDescriptionWidget>(dbmodel_,viewModels_),1);
	loginSignal().connect(descriptionWidget_,&ProblemDescriptionWidget::login);
	logoutSignal().connect(descriptionWidget_,&ProblemDescriptionWidget::logout);

	menuWidget_ = menuLayout->addWidget(cpp14::make_unique<ProblemSidemenuWidget>(dbmodel_,viewModels_),0);
	loginSignal().connect(menuWidget_,&ProblemSidemenuWidget::login);
	logoutSignal().connect(menuWidget_,&ProblemSidemenuWidget::logout);
	menuWidget_->setWidth(350);

	statisticsDialog_ = addChild(cpp14::make_unique<ProblemStatisticsDialog>(dbmodel_,viewModels_));
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

	problemData_ = dbmodel_->getProblem(id);

	if(problemData_.get() == nullptr) {
		pageTitle_->setText("Problem not found");
		return;
	}

	pageTitle_->setText(std::to_string(id) + " - " + problemData_->title);

	descriptionWidget_->setProblem(problemData_);
	menuWidget_->setProblem(problemData_);
}

void ProblemWidget::showSubmissionDialog() {

	submissionDialog_ = addChild(cpp14::make_unique<ProblemSubmissionDialog>(dbmodel_,viewModels_,session_,login_));
	submissionDialog_->setProblem(problemData_);

	submissionDialog_->finished().connect(this,&ProblemWidget::closeSubmissionDialog);
	submissionDialog_->show();
}

void ProblemWidget::closeSubmissionDialog(DialogCode code) {

	switch(code) {
	case DialogCode::Accepted:
		break;
	case DialogCode::Rejected:
		WStringStream strm;

		strm << "sessionStorage.setItem('OJdraft" << problemData_.id() << "',";
		strm << "'" << Utils::base64Encode(Utils::urlEncode(submissionDialog_->code()),false) << "');";

		doJavaScript(strm.str());
		break;
	}

	removeChild(submissionDialog_);
}

void ProblemWidget::showStatisticsDialog() {

	statisticsDialog_ = addChild(cpp14::make_unique<ProblemStatisticsDialog>(dbmodel_,viewModels_));
	statisticsDialog_->setProblem(problemData_);

	statisticsDialog_->finished().connect(this,&ProblemWidget::closeStatisticsDialog);
	statisticsDialog_->show();
}

void ProblemWidget::closeStatisticsDialog() {

	removeChild(statisticsDialog_);
}

ProblemDescriptionWidget::ProblemDescriptionWidget(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel),viewModels_(viewModels) {

	problemViewer_ = addWidget(cpp14::make_unique<OJProblemViewerWidget>());
}

void ProblemDescriptionWidget::login(Auth::Login& login) {

}

void ProblemDescriptionWidget::logout() {

}

void ProblemDescriptionWidget::setProblem(dbo::ptr<Problem> problemData) {

	Dbo::Transaction transaction = dbmodel_->startTransaction();
	dbo::ptr<Description> desc = problemData->description;
	problemViewer_->setContent(desc->htmldata.value());
}

ProblemSidemenuWidget::ProblemSidemenuWidget(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel), viewModels_(viewModels) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());

	auto submitButton = mainLayout->addWidget(cpp14::make_unique<WPushButton>("Submit solution"),0);
	submitButton->clicked().connect( [=] {
		showDialog_.emit(ProblemWidgetDialog::Submission);
	});

	auto downloadBookmarkLayout = mainLayout->addLayout(cpp14::make_unique<WHBoxLayout>(),0);
	downloadBookmarkLayout->setContentsMargins(0,0,0,0);

	downloadButton_ = downloadBookmarkLayout->addWidget(cpp14::make_unique<WPushButton>("Download PDF"),0);
	downloadButton_->setLink(WLink(std::make_shared<PdfResource>(dbmodel_)));
	downloadButton_->setIcon(WLink("images/pdf.svg"));

	// ------------- To be shown only if an user is logged in
	auto bookmarkButton = downloadBookmarkLayout->addWidget(cpp14::make_unique<WPushButton>("Bookmark problem"),0);
	bookmarkButton->setCheckable(true);
	bookmarkButton->setIcon(WLink("images/bookmark.svg"));

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

	auto rateLabel = mainLayout->addWidget(cpp14::make_unique<WText>("Rate this problem"));
	rateLabel->setTextAlignment(AlignmentFlag::Center);

	auto rateLayout = mainLayout->addLayout(cpp14::make_unique<WHBoxLayout>());
	rateLayout->setContentsMargins(0,0,0,0);
	rateLayout->addStretch(1);
	auto rateSet = rateLayout->addWidget(cpp14::make_unique<OJRatingSetWidget>(),0);
	rateLayout->addStretch(1);

	mainLayout->addStretch(1);
}

void ProblemSidemenuWidget::login(Auth::Login& login) {

}

void ProblemSidemenuWidget::logout() {

}

void ProblemSidemenuWidget::setProblem(dbo::ptr<Problem> problemData) {

	PdfResource *tmpPdfRes = (PdfResource*)downloadButton_->link().resource().get();
	tmpPdfRes->setProblem(problemData);
}

ProblemStatisticsDialog::ProblemStatisticsDialog(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel), viewModels_(viewModels) {
	setWindowTitle("Statistics dialog");
	setClosable(true);

	contents()->addWidget(cpp14::make_unique<WText>("Statistics stuff"));
}

void ProblemStatisticsDialog::login(Auth::Login& login) {

}

void ProblemStatisticsDialog::logout() {

}

void ProblemStatisticsDialog::setProblem(dbo::ptr<Problem> problemData) {

}

ProblemSubmissionDialog::ProblemSubmissionDialog(DBModel *dbmodel, ViewModels *viewModels, Session *session, Auth::Login *login)
	: dbmodel_(dbmodel), viewModels_(viewModels), session_(session), login_(login) {
	setWindowTitle("Submit");
	setClosable(true);
	resize(1024,700);

	codeEditor_ = contents()->addWidget(cpp14::make_unique<OJCodeEditorWidget>());
	codeEditor_->resize(774,530);
	codeEditor_->settingsChanged().connect(this,&ProblemSubmissionDialog::saveSettings);

	Dbo::Transaction transaction = dbmodel_->startTransaction();
	dbo::ptr<User> userData = session_->user(login_->user());
	OJCodeEditorSettings settings;

	settings.fontsize = userData->settings->editor_fontsize.value_or(16);
	settings.indent = userData->settings->editor_indent.value_or(4);
	settings.wrap = userData->settings->editor_wrap.value_or(true);
	settings.theme = userData->settings->editor_theme.value_or("textmate");

	codeEditor_->setSettings(settings);

	WPushButton *submit = footer()->addWidget(cpp14::make_unique<WPushButton>("Submit"));
	WPushButton *cancel = footer()->addWidget(cpp14::make_unique<WPushButton>("Cancel"));

	submit->clicked().connect(this,&WDialog::accept);
	cancel->clicked().connect(this,&WDialog::reject);
}

void ProblemSubmissionDialog::setProblem(dbo::ptr<Problem> problemData) {

	setWindowTitle(std::to_string(problemData.id())  + " - " + problemData->title);

	codeEditor_->loadCodeFromSession("OJdraft" + std::to_string(problemData.id()));
}

std::string ProblemSubmissionDialog::code() {

	return codeEditor_->code();
}

void ProblemSubmissionDialog::setCode(std::string code) {
	codeEditor_->setCode(code);
}

void ProblemSubmissionDialog::saveSettings(OJCodeEditorSettings& settings) {

	Dbo::Transaction transaction = dbmodel_->startTransaction();
	dbo::ptr<User> userData = session_->user(login_->user());

	userData->settings.modify()->editor_fontsize = settings.fontsize;
	userData->settings.modify()->editor_indent = settings.indent;
	userData->settings.modify()->editor_wrap = settings.wrap;
	userData->settings.modify()->editor_theme = settings.theme;
}
