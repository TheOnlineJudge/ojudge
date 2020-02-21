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
#include <Wt/WMessageBox.h>
#include <Wt/WContainerWidget.h>
#include "AdminCategoryWidget.h"

using namespace Wt;

AdminCategoryWidget::AdminCategoryWidget(const std::shared_ptr<CategoryModel> catmodel) : catmodel_(catmodel) {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout_->setContentsMargins(0,0,0,0);

	auto toolbarWidget = mainLayout_->addWidget(cpp14::make_unique<WContainerWidget>());
	auto toolbarLayout = toolbarWidget->setLayout(cpp14::make_unique<WHBoxLayout>());

	toolbarLayout->addStretch(1);
	auto addButton = toolbarLayout->addWidget(cpp14::make_unique<WImage>("images/add-button.svg"));
	addButton->setHeight(WLength(32));
	addButton->decorationStyle().setCursor(Cursor::PointingHand);
	addButton->setToolTip(WString("Add new category"));
	addButton->clicked().connect( [this] { showAddEditDialog(); });

	treeWidget_ = mainLayout_->addWidget(cpp14::make_unique<WTreeView>());

	treeWidget_->setModel(catmodel_);

	treeWidget_->setRowHeight(26);
	treeWidget_->setHeaderHeight(26);
	treeWidget_->setSortingEnabled(false);
	auto adminActionsDelegate = std::make_shared<AdminActionsDelegate>();
	adminActionsDelegate->editCategory().connect(this,&AdminCategoryWidget::showAddEditDialog);
	treeWidget_->setItemDelegateForColumn(3,adminActionsDelegate);
	treeWidget_->addStyleClass("myAdminCategoryTree");
}

void AdminCategoryWidget::showAddEditDialog(const WModelIndex& index) {

	std::string dialogTitle;

	if(index.isValid()) {
		dialogTitle = "Edit category";
	} else {
		dialogTitle = "Add new category";
	}
	addEditDialog_ = addChild(cpp14::make_unique<WDialog>(dialogTitle));

	auto result = addEditDialog_->contents()->addWidget(cpp14::make_unique<WTemplate>(WString::tr("addCategory-template")));
	result->addFunction("id",&WTemplate::Functions::id);

	auto selectParent = cpp14::make_unique<WTreeView>();
	selectParent->setModel(catmodel_);
	selectParent->setSortingEnabled(false);
	selectParent->setColumnHidden(1,true);
	selectParent->setColumnHidden(2,true);
	selectParent->setColumnHidden(3,true);
	selectParent->setSelectionMode(SelectionMode::Single);
	selectParent->setHeight(300);
	selectParent_ = selectParent.get();
	auto rootCheckBox = cpp14::make_unique<WCheckBox>("Root category");
	rootCheckBox->checked().connect( [=] { selectParent_->setHidden(true); } );
	rootCheckBox->unChecked().connect( [=] { selectParent_->setHidden(false); } );
	rootCheckBox_ = rootCheckBox.get();
	auto title = cpp14::make_unique<WLineEdit>();
	title_ = title.get();
	
	result->bindString("treelabel",WString("Select category parent"));
	result->bindString("titlelabel",WString("Title"));
	result->bindWidget("rootcheckbox",std::move(rootCheckBox));
	result->bindWidget("tree",std::move(selectParent));
	result->bindWidget("title",std::move(title));

	WPushButton *ok = addEditDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Save"));
	WPushButton *cancel = addEditDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Cancel"));

	ok->clicked().connect(addEditDialog_, &WDialog::accept);
	cancel->clicked().connect(addEditDialog_, &WDialog::reject);

	if(index.isValid()) {
		title_->setText(cpp17::any_cast<std::string>(catmodel_->data(index)));
		if(!index.parent().isValid()) {
			rootCheckBox_->setChecked(true);
			selectParent_->setHidden(true);
		} else {
			WModelIndex tmpIndex = index.parent().parent();
			while(tmpIndex.isValid()) {
				selectParent_->expand(tmpIndex);
				tmpIndex = tmpIndex.parent();
			}
			selectParent_->select(index.parent());
		}
	}

	addEditDialog_->finished().connect(this,&AdminCategoryWidget::addEditDialogDone);
	addEditDialog_->show();
}

void AdminCategoryWidget::addEditDialogDone(DialogCode code) {
	int parentId = 0;
	if(code == DialogCode::Accepted) {
		WModelIndex tmpIndex ;
		if(rootCheckBox_->isChecked()) {
			tmpIndex = selectParent_->rootIndex();
		} else if(selectParent_->selectedIndexes().size()) {
			WModelIndexSet::const_iterator tmpIterator = selectParent_->selectedIndexes().begin();
			tmpIndex = *tmpIterator;
		} else {
			WMessageBox::show("Error","You have to select one parent.",StandardButton::Ok);
			addEditDialog_->show();
			return;
		}
		catmodel_->addCategory(title_->text().toUTF8(),tmpIndex);
	}
	removeChild(addEditDialog_);
}

AdminActionsDelegate::AdminActionsDelegate() {

}

std::unique_ptr<WWidget> AdminActionsDelegate::update(WWidget *widget, const WModelIndex& index, WFlags<ViewItemRenderFlag> flags) {

	WidgetRef widgetRef(widget);

	bool isNew = false;

	if(widgetRef.w) {
		widgetRef.w = nullptr;
	}

	if(!widgetRef.w) {
		isNew = true;
		widgetRef.created = std::unique_ptr<WWidget>(new WContainerWidget());
		WContainerWidget *t = static_cast<WContainerWidget*>(widgetRef.created.get());
		t->addStyleClass("actions");
		auto layout = t->setLayout(cpp14::make_unique<WHBoxLayout>());
		layout->setContentsMargins(0,4,0,4);
		auto edit = layout->addWidget(cpp14::make_unique<WImage>("images/edit.svg"));
		edit->setHeight(18);
		edit->decorationStyle().setCursor(Cursor::PointingHand);
		edit->setToolTip("Edit category");
		edit->clicked().connect( [=] { editCategory_.emit(index.model()->index(index.row(),0,index.parent())); });
		auto down = layout->addWidget(cpp14::make_unique<WImage>("images/arrow_down.svg"));
		down->setHeight(18);
		down->decorationStyle().setCursor(Cursor::PointingHand);
		down->setToolTip("Move category down");
		if(index.model()->rowCount(index.parent()) == index.row()+1) down->setDisabled(true);
		auto up = layout->addWidget(cpp14::make_unique<WImage>("images/arrow_up.svg"));
		up->setHeight(18);
		up->decorationStyle().setCursor(Cursor::PointingHand);
		up->setToolTip("Move category up");
		if(index.row()==0) up->setDisabled(true);
		auto trash = layout->addWidget(cpp14::make_unique<WImage>("images/trash.svg"));
		trash->setHeight(18);
		trash->decorationStyle().setCursor(Cursor::PointingHand);
		trash->setToolTip("Delete category");
	}

	if(isNew) {
		return std::move(widgetRef.created);
	} else {
		return nullptr;
	}
		
}
