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
#include "ProblemWidget.h"
#include "PdfResource.h"
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

	menuWidget_ = menuLayout->addWidget(cpp14::make_unique<ProblemSidemenuWidget>(dbmodel_,viewModels_),0);

	menuWidget_->setWidth(350);
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

	menuWidget_->setProblem(id);
}

ProblemSidemenuWidget::ProblemSidemenuWidget(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel), viewModels_(viewModels) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());

	auto submitButton = mainLayout->addWidget(cpp14::make_unique<WPushButton>("Submit solution"),0);
	downloadButton_ = mainLayout->addWidget(cpp14::make_unique<WPushButton>("Download PDF version"),0);
	downloadButton_->setLink(WLink(std::make_shared<PdfResource>(dbmodel_)));

	mainLayout->addStretch(1);
}

void ProblemSidemenuWidget::setProblem(long long id) {

	PdfResource *tmpPdfRes = (PdfResource*)downloadButton_->link().resource().get();
	tmpPdfRes->setProblemId(id);
}
