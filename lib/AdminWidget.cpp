/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WTabWidget.h>
#include "authmodel/Session.h"
#include "viewmodels/ViewModels.h"
#include "AdminCategoryWidget.h"
#include "AdminWidget.h"

using namespace Wt;

AdminWidget::AdminWidget(Session *session,ViewModels *viewModels) : session_(session),viewModels_(viewModels) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	auto pageTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Admin"),0);
	pageTitle->addStyleClass("pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

	WTabWidget *tabWidget = mainWidget->addNew<WTabWidget>();

	tabWidget->addTab(cpp14::make_unique<WText>("Problems tab"),"Problems",ContentLoading::Lazy);
	tabWidget->addTab(cpp14::make_unique<AdminCategoryWidget>(viewModels->getCategoryModel()),"Categories",ContentLoading::Lazy);
	tabWidget->addTab(cpp14::make_unique<WText>("Contests tab"),"Contests",ContentLoading::Lazy);
	tabWidget->addTab(cpp14::make_unique<WText>("Users tab"),"Users",ContentLoading::Lazy);
	tabWidget->addTab(cpp14::make_unique<WText>("Languages tab"),"Languages",ContentLoading::Lazy);
	tabWidget->addTab(cpp14::make_unique<WText>("Settings tab"),"Settings",ContentLoading::Lazy);

}

