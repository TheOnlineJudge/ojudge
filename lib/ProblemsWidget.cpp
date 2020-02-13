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
#include <Wt/WCssDecorationStyle.h>
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

	auto categoriesWidget = menuLayout->addWidget(cpp14::make_unique<WTreeView>(),0);
	categoriesWidget->addStyleClass("myProblemsCategories");
	categoriesWidget->decorationStyle().setCursor(Cursor::PointingHand);
	categoriesWidget->setModel(viewModels_->getCategoryModel());
	categoriesWidget->setWidth(WLength(300)) ;
	categoriesWidget->setColumnHidden(1,true);
	categoriesWidget->setColumnHidden(2,true);
	categoriesWidget->setColumnHidden(3,true);
	categoriesWidget->clicked().connect(this,&ProblemsWidget::categoryClicked) ;

	auto problemsWidget = menuLayout->addWidget(cpp14::make_unique<WTableView>(),1);
	proxyModel_ = std::make_shared<WSortFilterProxyModel>();
	proxyModel_->setSourceModel(viewModels_->getProblemModel());
	proxyModel_->setFilterKeyColumn(3);

	problemsWidget->setModel(proxyModel_);
	problemsWidget->setRowHeight(26);
	problemsWidget->setHeaderHeight(26);
	problemsWidget->addStyleClass("myProblemsProblems");
	problemsWidget->setColumnHidden(3,true);
	
}

void ProblemsWidget::categoryClicked(WModelIndex modelIndex, WMouseEvent mouseEvent) {

	proxyModel_->setFilterRegExp(std::make_unique<std::regex>(asString(modelIndex.data(CategoryModel::CategoryIdRole),WString(".*#%d#.*")).toUTF8())) ;

}
