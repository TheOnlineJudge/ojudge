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
#include <Wt/WPushButton.h>
#include "ProblemWidget.h"
#include "dbmodel/DBModel.h"
#include "base64.h"
#include "widgets/OJProblemViewerWidget.h"

using namespace Wt;

ProblemWidget::ProblemWidget(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel),viewModels_(viewModels) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	pageTitle_ = mainLayout->addWidget(cpp14::make_unique<WText>("Problem"),0);
	pageTitle_->addStyleClass("pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

	auto menuLayout = mainWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
	menuLayout->setContentsMargins(0,0,0,0);

	problemViewer_ = menuLayout->addWidget(cpp14::make_unique<OJProblemViewerWidget>(),1);

	auto menuWidget = cpp14::make_unique<ProblemSidemenuWidget>(dbmodel_,viewModels_);
	menuWidget->setWidth(350);
	menuLayout->addWidget(std::move(menuWidget),0);
}

void ProblemWidget::setProblem(long long id) {

	dbo::ptr<Problem> problemData = dbmodel_->getProblem(id);

	if(problemData.get() == nullptr) {
		pageTitle_->setText("Problem not found");
		return;
	}

	pageTitle_->setText(std::to_string(id) + " - " + problemData->title);

	Dbo::Transaction transaction = dbmodel_->startTransaction();
	dbo::ptr<Description> desc = problemData->description;
	problemViewer_->setContent(desc->htmldata.value());
}

ProblemSidemenuWidget::ProblemSidemenuWidget(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel), viewModels_(viewModels) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());

	auto submitButton = mainLayout->addWidget(cpp14::make_unique<WPushButton>("Submit solution"),0);
	auto downloadButton = mainLayout->addWidget(cpp14::make_unique<WPushButton>("Download PDF version"),0);

	mainLayout->addStretch(1);

}

void ProblemSidemenuWidget::setProblem(long long id) {

}
