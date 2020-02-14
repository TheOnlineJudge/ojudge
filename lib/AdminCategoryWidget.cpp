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
#include <Wt/WHBoxLayout.h>
#include <Wt/WImage.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WTemplate.h>
#include "AdminCategoryWidget.h"

using namespace Wt;

AdminCategoryWidget::AdminCategoryWidget(const std::shared_ptr<CategoryModel> catmodel) : catmodel_(catmodel) {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout_->setContentsMargins(0,0,0,0);

	auto toolbarWidget = mainLayout_->addWidget(cpp14::make_unique<WContainerWidget>());
	auto toolbarLayout = toolbarWidget->setLayout(cpp14::make_unique<WHBoxLayout>());

	auto addButton = toolbarLayout->addWidget(cpp14::make_unique<WImage>("images/add-button.svg"));
	addButton->setHeight(WLength(32));
	addButton->decorationStyle().setCursor(Cursor::PointingHand);
	addButton->setToolTip(WString("Add new category"));
	addButton->clicked().connect(this,&AdminCategoryWidget::showAddDialog);
	toolbarLayout->addStretch(1);

	treeWidget_ = mainLayout_->addWidget(cpp14::make_unique<WTreeView>());

	treeWidget_->setModel(catmodel_);

	treeWidget_->setRowHeight(26);
	treeWidget_->setHeaderHeight(26);
	treeWidget_->setSortingEnabled(false);
}

void AdminCategoryWidget::showAddDialog() {

	addDialog_ = addChild(cpp14::make_unique<WDialog>("Add new category"));

	auto result = addDialog_->contents()->addWidget(cpp14::make_unique<WTemplate>(WString::tr("addCategory-template")));
	result->addFunction("id",&WTemplate::Functions::id);

	auto selectParent = cpp14::make_unique<WTreeView>();
	selectParent->setModel(catmodel_);
	selectParent->setSortingEnabled(false);
	selectParent->setColumnHidden(1,true);
	selectParent->setColumnHidden(2,true);
	selectParent->setColumnHidden(3,true);
	selectParent->setSelectionMode(SelectionMode::Single);
	selectParent_ = selectParent.get();
	auto rootCheckBox = cpp14::make_unique<WCheckBox>("Root category");
	rootCheckBox->checked().connect( [=] { selectParent_->setHidden(true); } );
	rootCheckBox->unChecked().connect( [=] { selectParent_->setHidden(false); } );
	rootCheckBox_ = rootCheckBox.get();
	auto newTitle = cpp14::make_unique<WLineEdit>();
	newTitle_ = newTitle.get();
	

	result->bindString("treelabel",WString("Select category parent"));
	result->bindString("titlelabel",WString("Title"));
	result->bindWidget("rootcheckbox",std::move(rootCheckBox));
	result->bindWidget("tree",std::move(selectParent));
	result->bindWidget("title",std::move(newTitle));

	WPushButton *ok = addDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Ok"));
	WPushButton *cancel = addDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Cancel"));

	ok->clicked().connect(addDialog_, &WDialog::accept);
	cancel->clicked().connect(addDialog_, &WDialog::reject);

	addDialog_->finished().connect(this,&AdminCategoryWidget::addDialogDone);
	addDialog_->show();
}

void AdminCategoryWidget::addDialogDone(DialogCode code) {
	removeChild(addDialog_);
}
