/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#include <sstream>
#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WTreeView.h>
#include <Wt/WAnimation.h>
#include "ProblemsWidget.h"
#include "viewmodels/ViewModels.h"

using namespace Wt;

ProblemsWidget::ProblemsWidget(ViewModels *viewModels) : viewModels_(viewModels) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	auto pageTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Problems"),0);
	pageTitle->addStyleClass("pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

	auto menuLayout = mainWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
	menuLayout->setContentsMargins(0,0,0,0);

	auto menuWidget = menuLayout->addWidget(cpp14::make_unique<WTreeView>(),0);
	menuWidget->addStyleClass("myProblemsCategories");
	menuWidget->setModel(viewModels_->getCategoryModel());
	menuWidget->setWidth(WLength(300)) ;
	menuWidget->setColumnHidden(1,true);
	menuWidget->setColumnHidden(2,true);
	menuWidget->setColumnHidden(3,true);

	auto problemsWidget = menuLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);
	problemsWidget->addStyleClass("tmpproblems");
}
