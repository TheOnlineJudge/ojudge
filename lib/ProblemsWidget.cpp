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
#include "ojudgeApp.h"
#include "ProblemsWidget.h"
#include "viewmodels/ViewModels.h"
#include "widgets/OJRatingViewWidget.h"

using namespace Wt;

ProblemsWidget::ProblemsWidget(ViewModels *viewModels) : viewModels_(viewModels) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	auto pageTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Problems"),0);
	pageTitle->addStyleClass("oj-pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

	auto menuLayout = mainWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
	menuLayout->setContentsMargins(0,0,0,0);

	auto categoriesWidget = menuLayout->addWidget(cpp14::make_unique<WTreeView>(),0);
	categoriesWidget->addStyleClass("oj-problems-categories");
	categoriesWidget->decorationStyle().setCursor(Cursor::PointingHand);
	categoriesWidget->setModel(viewModels_->getCategoryModel());
	categoriesWidget->setWidth(WLength(300));
	categoriesWidget->setColumnHidden(1,true);
	categoriesWidget->setColumnHidden(2,true);
	categoriesWidget->setColumnHidden(3,true);
	categoriesWidget->clicked().connect(this,&ProblemsWidget::categoryClicked);

	auto problemsWidget = menuLayout->addWidget(cpp14::make_unique<WTableView>(),1);
	proxyModel_ = std::make_shared<WSortFilterProxyModel>();
	proxyModel_->setSourceModel(viewModels_->getProblemModel());
	proxyModel_->setDynamicSortFilter(true);
	proxyModel_->setFilterKeyColumn(3);

	problemsWidget->setModel(proxyModel_);
	problemsWidget->setRowHeight(52);
	problemsWidget->setHeaderHeight(26);
	problemsWidget->addStyleClass("oj-problems-problems");
	problemsWidget->setColumnHidden(1,true);
	problemsWidget->setColumnHidden(2,true);
	problemsWidget->setColumnHidden(3,true);
	problemsWidget->setColumnWidth(0,921);
	problemsWidget->decorationStyle().setCursor(Cursor::PointingHand);
	problemsWidget->setSortingEnabled(false);
	problemsWidget->clicked().connect(this,&ProblemsWidget::problemClicked);

	auto problemDelegate = std::make_shared<ProblemDelegate>();
	problemsWidget->setItemDelegateForColumn(0,problemDelegate);

}

void ProblemsWidget::categoryClicked(WModelIndex modelIndex, WMouseEvent mouseEvent) {

	proxyModel_->setFilterRegExp(std::make_unique<std::regex>(asString(modelIndex.data(CategoryModel::CategoryIdRole),WString(".*#%d#.*")).toUTF8()));

}

void ProblemsWidget::problemClicked(WModelIndex modelIndex, WMouseEvent mouseEvent) {

	auto app = dynamic_cast<ojudgeApp*>(Wt::WApplication::instance());
	app->setInternalPath("/problem/"+asString(modelIndex.data()).toUTF8(),true);

}

ProblemsWidget::ProblemDelegate::ProblemDelegate() {

}

std::unique_ptr<WWidget> ProblemsWidget::ProblemDelegate::update(WWidget *widget, const WModelIndex& index, WFlags<ViewItemRenderFlag> flags) {

	WidgetRef widgetRef(widget);

	bool isNew = false;

	if(widgetRef.w) {
		widgetRef.w = nullptr;
	}

	if(!widgetRef.w) {
		isNew = true;
		widgetRef.created = std::unique_ptr<WWidget>(new WContainerWidget());
                WContainerWidget *t = static_cast<WContainerWidget*>(widgetRef.created.get());
		t->setMargin(0);
                t->addStyleClass("oj-problems-problems-item");

		auto layout = t->setLayout(cpp14::make_unique<WHBoxLayout>());
		layout->setContentsMargins(3,3,3,3);

		auto problemId = layout->addWidget(cpp14::make_unique<WText>(std::to_string(cpp17::any_cast<long long>(index.data()))),0);
		problemId->addStyleClass("oj-problems-problems-id");

		auto centerLayout = layout->addLayout(cpp14::make_unique<WVBoxLayout>(),1);
		centerLayout->setContentsMargins(0,0,0,0);

		auto problemTitle = centerLayout->addWidget(cpp14::make_unique<WText>("This is the problem title"),0);
		problemTitle->setMargin(0);
		problemTitle->addStyleClass("oj-problems-problems-title");

		auto problemStats = centerLayout->addWidget(cpp14::make_unique<WText>("Problem stats here"),0);
		problemStats->setMargin(0);
		problemStats->addStyleClass("oj-problems-problems-stats");

		auto problemRating = layout->addWidget(cpp14::make_unique<OJRatingViewWidget>(),0);
		problemRating->setRating(50);
	}

        if(isNew) {
                return std::move(widgetRef.created);
        } else {
                return nullptr;
        }
}
