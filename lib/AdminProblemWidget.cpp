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

	auto problemSelectorWidget = toolbarLayout->addWidget(cpp14::make_unique<WTemplate>(WString::tr("lineEdit-template")));
	problemSelectorWidget->addFunction("id",&WTemplate::Functions::id);
	auto problemSelector = cpp14::make_unique<WLineEdit>();
	problemSelector_ = problemSelector.get();
	problemSelectorWidget->bindString("label","Problem");
	problemSelectorWidget->bindWidget("edit",std::move(problemSelector));

	problemSelector_->setValidator(std::make_shared<Wt::WIntValidator>());
	problemSelector_->textInput().connect(this,&AdminProblemWidget::problemSelectorSlot);

	auto categoryComboWidget = toolbarLayout->addWidget(cpp14::make_unique<WTemplate>(WString::tr("comboBox-template")));
	categoryComboWidget->addFunction("id",&WTemplate::Functions::id);

	auto categoryCombo = cpp14::make_unique<WComboBox>();
	categoryCombo->setModel(viewModels_->getCategoryModel());

	categoryComboWidget->bindString("label","Category");
	categoryComboWidget->bindWidget("combo",std::move(categoryCombo));

	toolbarLayout->addStretch(1);

	auto addButton = toolbarLayout->addWidget(cpp14::make_unique<WImage>("images/add-button.svg"));
	addButton->setHeight(WLength(32));
	addButton->decorationStyle().setCursor(Cursor::PointingHand);
	addButton->setToolTip(WString("Add new problem"));
	addButton->clicked().connect(this,&AdminProblemWidget::showAddDialog);

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

void AdminProblemWidget::showAddDialog() {

	addDialog_ = addChild(cpp14::make_unique<WDialog>("Add new problem"));

	auto result = addDialog_->contents()->addWidget(cpp14::make_unique<WTemplate>(WString::tr("addProblem-template")));
        result->addFunction("id",&WTemplate::Functions::id);

	auto newId = cpp14::make_unique<WLineEdit>();
	newId->setValidator(std::make_shared<Wt::WIntValidator>());
	newId_ = newId.get();

	auto newTitle = cpp14::make_unique<WLineEdit>();
	newTitle_ = newTitle.get();

	auto newDescription = cpp14::make_unique<WFileUpload>();
	newDescription_ = newDescription.get();

	auto newCategories = cpp14::make_unique<WTreeView>();
	newCategories->setModel(viewModels_->getCategoryModel()) ;
	newCategories->setColumnHidden(1,true);
	newCategories->setColumnHidden(2,true);
	newCategories->setColumnHidden(3,true);
	newCategories->setSelectionMode(SelectionMode::Extended);
	newCategories_ = newCategories.get();

	result->bindString("idlabel",WString("ID"));
	result->bindString("titlelabel",WString("Title"));
	result->bindString("descriptionlabel",WString("Problem description"));
	result->bindString("categorieslabel",WString("Categories"));
	result->bindWidget("id",std::move(newId));
	result->bindWidget("title",std::move(newTitle));
	result->bindWidget("description",std::move(newDescription));
	result->bindWidget("categories",std::move(newCategories));

	WPushButton *ok = addDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Save"));
        WPushButton *cancel = addDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Cancel"));

	ok->clicked().connect(addDialog_, &WDialog::accept);
        cancel->clicked().connect(addDialog_, &WDialog::reject);

        addDialog_->finished().connect(this,&AdminProblemWidget::addDialogDone);
        addDialog_->show();

}

void AdminProblemWidget::addDialogDone(DialogCode code) {
	removeChild(addDialog_);
}
