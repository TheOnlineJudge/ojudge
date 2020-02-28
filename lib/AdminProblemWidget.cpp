/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <fstream>
#include <iterator>
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
#include <Wt/WProgressBar.h>
#include "AdminProblemWidget.h"

using namespace Wt;

AdminProblemWidget::AdminProblemWidget(ViewModels *viewModels, DBModel *dbmodel) : viewModels_(viewModels), dbmodel_(dbmodel) {

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
	addButton->clicked().connect( [this] { showAddEditDialog(); });

	tableWidget_ = mainLayout_->addWidget(cpp14::make_unique<WTableView>());

	proxyModel_ = std::make_shared<WSortFilterProxyModel>();
	proxyModel_->setSourceModel(viewModels_->getProblemModel());
	proxyModel_->setDynamicSortFilter(true);

	tableWidget_->setModel(proxyModel_);

	tableWidget_->setRowHeight(26);
	tableWidget_->setHeaderHeight(26);
	tableWidget_->setSortingEnabled(false);
	tableWidget_->setColumnHidden(3,true);

	auto adminActionsDelegate = std::make_shared<AdminProblemWidget::AdminActionsDelegate>();
	adminActionsDelegate->editProblem().connect(this,&AdminProblemWidget::showAddEditDialog);
	tableWidget_->setItemDelegateForColumn(2,adminActionsDelegate);
	
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

void AdminProblemWidget::showAddEditDialog(const WModelIndex& index) {

	std::string dialogTitle;

	if(!index.isValid()) {
		dialogTitle = "Add new problem";
	} else {
		dialogTitle = "Edit problem";
	}

	addDialog_ = addChild(cpp14::make_unique<WDialog>(dialogTitle));

	auto result = addDialog_->contents()->addWidget(cpp14::make_unique<WTemplate>(WString::tr("addProblem-template")));
        result->addFunction("id",&WTemplate::Functions::id);

	auto id = cpp14::make_unique<WLineEdit>();
	id->setValidator(std::make_shared<Wt::WIntValidator>());
	id_ = id.get();

	auto title = cpp14::make_unique<WLineEdit>();
	title_ = title.get();

	auto description = cpp14::make_unique<WFileUpload>();
	description_ = description.get();
	description_->changed().connect(description_, &WFileUpload::upload);

	auto progress = cpp14::make_unique<WProgressBar>();
	WProgressBar *progress_ = progress.get() ;
	description_->setProgressBar(progress_);

	auto categories = cpp14::make_unique<WTreeView>();
	categories->setModel(viewModels_->getCategoryModel()) ;
	categories->setColumnHidden(1,true);
	categories->setColumnHidden(2,true);
	categories->setColumnHidden(3,true);
	categories->setSelectionMode(SelectionMode::Extended);
	categories_ = categories.get();

	result->bindString("idlabel",WString("ID"));
	result->bindString("titlelabel",WString("Title"));
	result->bindString("descriptionlabel",WString("Problem description"));
	result->bindString("categorieslabel",WString("Categories"));
	result->bindWidget("id",std::move(id));
	result->bindWidget("title",std::move(title));
	result->bindWidget("description",std::move(description));
	result->bindWidget("progress",std::move(progress));
	result->bindWidget("categories",std::move(categories));

	WPushButton *ok = addDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Save"));
        WPushButton *cancel = addDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Cancel"));

	ok->clicked().connect(addDialog_, &WDialog::accept);
        cancel->clicked().connect(addDialog_, &WDialog::reject);

/*	ok->disable();
	description_->uploaded().connect(ok, &WPushButton::enable);*/

        addDialog_->finished().connect(this,&AdminProblemWidget::addDialogDone);
        addDialog_->show();

}

void AdminProblemWidget::addDialogDone(DialogCode code) {
	if(code == DialogCode::Accepted) {
		viewModels_->getProblemModel()->addProblem(std::stoi(id_->text().toUTF8()),title_->text().toUTF8());
		std::ifstream descFile(description_->spoolFileName(), std::ios::binary);
		std::vector<unsigned char> descFileContents(std::istreambuf_iterator<char>{descFile},{});
		dbmodel_->updateDescription(std::stoi(id_->text().toUTF8()),descFileContents);
	}
		
	removeChild(addDialog_);
}

AdminProblemWidget::AdminActionsDelegate::AdminActionsDelegate() {

}

std::unique_ptr<WWidget> AdminProblemWidget::AdminActionsDelegate::update(WWidget *widget, const WModelIndex& index, WFlags<ViewItemRenderFlag> flags) {

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
                edit->setToolTip("Edit problem");
                edit->clicked().connect( [=] { editProblem_.emit(index.model()->index(index.row(),0,index.parent())); });
                auto trash = layout->addWidget(cpp14::make_unique<WImage>("images/trash.svg"));
                trash->setHeight(18);
                trash->decorationStyle().setCursor(Cursor::PointingHand);
                trash->setToolTip("Delete problem");
        }

        if(isNew) {
                return std::move(widgetRef.created);
        } else {
                return nullptr;
        }

}
