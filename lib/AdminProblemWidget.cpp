/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>
#include <Wt/WComboBox.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WTemplate.h>
#include <Wt/WIntValidator.h>
#include <Wt/WImage.h>
#include <Wt/WCssDecorationStyle.h>
#include "AdminProblemWidget.h"

using namespace Wt;

AdminProblemWidget::AdminProblemWidget(ViewModels *viewModels) : viewModels_(viewModels) {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout_->setContentsMargins(0,0,0,0);

	auto toolbarWidget = mainLayout_->addWidget(cpp14::make_unique<WContainerWidget>());
	auto toolbarLayout = toolbarWidget->setLayout(cpp14::make_unique<WHBoxLayout>());

	auto addButton = toolbarLayout->addWidget(cpp14::make_unique<WImage>("images/add-button.svg"));
	addButton->setHeight(WLength(32));
	addButton->decorationStyle().setCursor(Cursor::PointingHand);
	addButton->setToolTip(WString("Add new problem"));
	toolbarLayout->addStretch(1);
	auto categoryComboWidget = toolbarLayout->addWidget(cpp14::make_unique<WTemplate>(WString::tr("comboBox-template")));
	categoryComboWidget->addFunction("id",&WTemplate::Functions::id);
	auto categoryCombo = cpp14::make_unique<WComboBox>();
	categoryCombo->setModel(viewModels_->getCategoryModel());

	categoryComboWidget->bindString("label","Category");
	categoryComboWidget->bindWidget("combo",std::move(categoryCombo));

	auto problemSelectorWidget = toolbarLayout->addWidget(cpp14::make_unique<WTemplate>(WString::tr("lineEdit-template")));
	problemSelectorWidget->addFunction("id",&WTemplate::Functions::id);
	auto problemSelector = cpp14::make_unique<WLineEdit>();
	problemSelector_ = problemSelector.get();
	problemSelectorWidget->bindString("label","Problem");
	problemSelectorWidget->bindWidget("edit",std::move(problemSelector));

	problemSelector_->setValidator(std::make_shared<Wt::WIntValidator>());
	problemSelector_->textInput().connect(this,&AdminProblemWidget::problemSelectorSlot);

	tableWidget_ = mainLayout_->addWidget(cpp14::make_unique<WTableView>());

	proxyModel_ = std::make_shared<WSortFilterProxyModel>();
	proxyModel_->setSourceModel(viewModels_->getProblemModel());

	tableWidget_->setModel(proxyModel_);

	tableWidget_->setRowHeight(26);
	tableWidget_->setHeaderHeight(26);
	tableWidget_->setSortingEnabled(false);
	tableWidget_->setColumnHidden(3,true);
}

void AdminProblemWidget::problemSelectorSlot() {

	proxyModel_->setFilterKeyColumn(0);
	proxyModel_->setFilterRole(ItemDataRole::Display);
	if(problemSelector_->text().toUTF8() == "") {
		proxyModel_->setFilterRegExp(std::make_unique<std::regex>(".*"));
	} else {
		proxyModel_->setFilterRegExp(std::make_unique<std::regex>(problemSelector_->text().toUTF8()));
	}
}
