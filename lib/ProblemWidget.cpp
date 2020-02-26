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
#include "dbmodel/DBModel.h"
#include "base64.h"

using namespace Wt;

ProblemWidget::ProblemWidget(DBModel *dbmodel, ViewModels *viewModels) : dbmodel_(dbmodel),viewModels_(viewModels) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	pageTitle_ = mainLayout->addWidget(cpp14::make_unique<WText>("Problem"),0);
	pageTitle_->addStyleClass("pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

	auto menuLayout = mainWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
	menuLayout->setContentsMargins(0,0,0,0);

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
	// std::cout << "DEBUG data: " << std::endl << base64_encode(desc->pdfdata.data(),desc->pdfdata.size()) << std::endl;
}
