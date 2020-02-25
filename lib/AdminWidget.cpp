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
#include "dbmodel/DBModel.h"
#include "viewmodels/ViewModels.h"
#include "AdminCategoryWidget.h"
#include "AdminContestWidget.h"
#include "AdminLanguageWidget.h"
#include "AdminProblemWidget.h"
#include "AdminSettingsWidget.h"
#include "AdminWidget.h"

using namespace Wt;

AdminWidget::AdminWidget(Session *session,ViewModels *viewModels,DBModel *dbmodel) : session_(session),viewModels_(viewModels),dbmodel_(dbmodel) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	auto pageTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Admin"),0);
	pageTitle->addStyleClass("pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

	WTabWidget *tabWidget = mainWidget->addNew<WTabWidget>();

	tabWidget->addTab(cpp14::make_unique<AdminProblemWidget>(viewModels_,dbmodel_),"Problems",ContentLoading::Lazy);
	tabWidget->addTab(cpp14::make_unique<AdminCategoryWidget>(viewModels_->getCategoryModel()),"Categories",ContentLoading::Lazy);
	tabWidget->addTab(cpp14::make_unique<AdminContestWidget>(),"Contests",ContentLoading::Lazy);
	tabWidget->addTab(cpp14::make_unique<WText>("Users tab"),"Users",ContentLoading::Lazy);
	tabWidget->addTab(cpp14::make_unique<AdminLanguageWidget>(),"Languages",ContentLoading::Lazy);
	tabWidget->addTab(cpp14::make_unique<AdminSettingsWidget>(dbmodel_),"Settings",ContentLoading::Lazy);

}

