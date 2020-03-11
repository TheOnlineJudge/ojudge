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
#include "ProblemWidget.h"
#include "PdfResource.h"
#include "base64.h"
#include "widgets/OJProblemViewerWidget.h"
#include "widgets/OJCodeEditorWidget.h"

using namespace Wt;

ProblemWidget::ProblemWidget(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel),viewModels_(viewModels) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	pageTitle_ = mainLayout->addWidget(cpp14::make_unique<WText>("Problem"),0);
	pageTitle_->addStyleClass("pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

	auto menuLayout = mainWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
	menuLayout->setContentsMargins(0,0,0,0);

	descriptionWidget_ = menuLayout->addWidget(cpp14::make_unique<ProblemDescriptionWidget>(dbmodel_,viewModels_),1);

	menuWidget_ = menuLayout->addWidget(cpp14::make_unique<ProblemSidemenuWidget>(dbmodel_,viewModels_),0);
	menuWidget_->setWidth(350);

	statisticsDialog_ = addChild(cpp14::make_unique<ProblemStatisticsDialog>(dbmodel_,viewModels_));
// Not for milestone 1.0.0	discussionDialog_ = addChild(cpp14::make_unique<ProblemDiscussionDialog>(dbmodel_,viewModels_));

	menuWidget_->showDialog().connect( [=] (ProblemWidgetDialog dialog) {
		switch(dialog) {
/* Not for milestone 1.0.0		case ProblemWidgetDialog::Discussion:
			showDiscussionDialog();
			break;*/
		case ProblemWidgetDialog::Submission:
			showSubmissionDialog();
			break;
		case ProblemWidgetDialog::Statistics:
			showStatisticsDialog();
			break;
		}
	});

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

	submissionDialog_ = addChild(cpp14::make_unique<ProblemSubmissionDialog>(dbmodel_,viewModels_));
	submissionDialog_->setProblem(problemData_);

	submissionDialog_->finished().connect(this,&ProblemWidget::closeSubmissionDialog);
        submissionDialog_->show();
}

void ProblemWidget::closeSubmissionDialog(DialogCode code) {

	removeChild(submissionDialog_);
}

/* Not for milestone 1.0.0
void ProblemWidget::showDiscussionDialog() {

	discussionDialog_ = addChild(cpp14::make_unique<ProblemDiscussionDialog>(dbmodel_,viewModels_));
	discussionDialog_->setProblem(problemData_);

	discussionDialog_->finished().connect(this,&ProblemWidget::closeDiscussionDialog);
	discussionDialog_->show();
}

void ProblemWidget::closeDiscussionDialog() {

	removeChild(discussionDialog_);
}
*/

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

	downloadButton_ = mainLayout->addWidget(cpp14::make_unique<WPushButton>("Download PDF version"),0);
	downloadButton_->setLink(WLink(std::make_shared<PdfResource>(dbmodel_)));

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

/*	Not for milestone 1.0.0
	auto discussButton = mainLayout->addWidget(cpp14::make_unique<WPushButton>("Discuss problem"),0);
	discussButton->clicked().connect( [=] {
		showDialog_.emit(ProblemWidgetDialog::Discussion);
	});*/

	// ------------- To be shown only if an user is logged in
	auto starredButton = mainLayout->addWidget(cpp14::make_unique<WPushButton>("Starred problem"),0);
	starredButton->setCheckable(true);

	auto voteButtons = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0);
	auto voteLayout = voteButtons->setLayout(cpp14::make_unique<WHBoxLayout>());
	voteLayout->setContentsMargins(0,0,0,0);

	auto voteUpButton = voteLayout->addWidget(cpp14::make_unique<WPushButton>("Vote Up"),1);
	auto voteDownButton = voteLayout->addWidget(cpp14::make_unique<WPushButton>("Vote Down"),1);
	// -------------

	mainLayout->addStretch(1);
}

void ProblemSidemenuWidget::setProblem(dbo::ptr<Problem> problemData) {

	PdfResource *tmpPdfRes = (PdfResource*)downloadButton_->link().resource().get();
	tmpPdfRes->setProblem(problemData);
}

/* Not for milestone 1.0.0
ProblemDiscussionDialog::ProblemDiscussionDialog(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel), viewModels_(viewModels) {
	setWindowTitle("Discussion dialog");
	setClosable(true);
	setTransient(true);
}

void ProblemDiscussionDialog::setProblem(dbo::ptr<Problem> problemData) {

}
*/

ProblemStatisticsDialog::ProblemStatisticsDialog(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel), viewModels_(viewModels) {
	setWindowTitle("Statistics dialog");
	setClosable(true);
	setTransient(true);

	contents()->addWidget(cpp14::make_unique<WText>("Statistics stuff"));
}

void ProblemStatisticsDialog::setProblem(dbo::ptr<Problem> problemData) {

}

ProblemSubmissionDialog::ProblemSubmissionDialog(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel), viewModels_(viewModels) {
	setWindowTitle("Submit");
	setClosable(true);
	setTransient(true);
	resize(1024,700);

	auto codeEditor = contents()->addWidget(cpp14::make_unique<OJCodeEditorWidget>());
	codeEditor->resize(800,500);
}

void ProblemSubmissionDialog::setProblem(dbo::ptr<Problem> problemData) {

	setWindowTitle(std::to_string(problemData.id())  + " - " + problemData->title);
}
