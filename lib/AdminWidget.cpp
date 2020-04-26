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
#include <Wt/WHBoxLayout.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>
#include <Wt/WImage.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WMessageBox.h>
#include <Wt/WComboBox.h>
#include <Wt/WIntValidator.h>
#include <Wt/WProgressBar.h>
#include <Wt/WMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WApplication.h>
#include <Wt/Mail/Client.h>
#include <Wt/Mail/Message.h>
#include "dbmodel/DBModel.h"
#include "datastore/SettingStore.h"
#include "widgets/OJColorPicker.h"
#include "AdminWidget.h"

using namespace Wt;

AdminWidget::AdminWidget(Session *session,ViewModels *viewModels,DBModel *dbmodel,DataStore *dataStore) : session_(session),viewModels_(viewModels),dbmodel_(dbmodel),dataStore_(dataStore) {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout_->setContentsMargins(0,0,0,0);
	mainLayout_->setPreferredImplementation(LayoutImplementation::JavaScript);

	auto pageTitle = mainLayout_->addWidget(cpp14::make_unique<WText>("Admin"),0);
	pageTitle->addStyleClass("oj-pagetitle");

	mustLoginWidget_ = mainLayout_->addWidget(cpp14::make_unique<WText>("You are not logged in."),1);
}


void AdminWidget::login(Auth::Login& login) {

	dbo::Transaction transaction = dbmodel_->startTransaction();
	dbo::ptr<User> userData = session_->user(login.user());

	if(userData->role == Role::Admin) {

		mustLoginWidget_->hide();

		mainWidget_ = mainLayout_->addWidget(cpp14::make_unique<WContainerWidget>(),1);
		auto menuLayout = mainWidget_->setLayout(cpp14::make_unique<WHBoxLayout>());
		menuLayout->setContentsMargins(0,0,0,0);
		menuLayout->setPreferredImplementation(LayoutImplementation::JavaScript);

		auto mainStack = menuLayout->addWidget(cpp14::make_unique<WStackedWidget>(),1);
		auto mainMenu = menuLayout->insertWidget(0,cpp14::make_unique<WMenu>(mainStack),0);
		mainMenu->addStyleClass("flex-column");
		mainMenu->setWidth(200);
		auto settingsMenu = cpp14::make_unique<WMenu>(mainStack);

		auto adminProblemWidget = cpp14::make_unique<AdminProblemWidget>(viewModels_,dbmodel_);
		loginSignal().connect(adminProblemWidget.get(),&AdminProblemWidget::login);
		logoutSignal().connect(adminProblemWidget.get(),&AdminProblemWidget::logout);
		mainMenu->addItem("Problems",std::move(adminProblemWidget));

		auto adminCategoryWidget = cpp14::make_unique<AdminCategoryWidget>(viewModels_->getCategoryModel());
		loginSignal().connect(adminCategoryWidget.get(),&AdminCategoryWidget::login);
		logoutSignal().connect(adminCategoryWidget.get(),&AdminCategoryWidget::logout);
		mainMenu->addItem("Categories",std::move(adminCategoryWidget));

		auto adminContestWidget = cpp14::make_unique<AdminContestWidget>();
		loginSignal().connect(adminContestWidget.get(),&AdminContestWidget::login);
		logoutSignal().connect(adminContestWidget.get(),&AdminContestWidget::logout);
		mainMenu->addItem("Contests",std::move(adminContestWidget));

		auto adminUserWidget = cpp14::make_unique<AdminUserWidget>(viewModels_,dbmodel_);
		loginSignal().connect(adminUserWidget.get(),&AdminUserWidget::login);
		logoutSignal().connect(adminUserWidget.get(),&AdminUserWidget::logout);
		mainMenu->addItem("Users",std::move(adminUserWidget));

		auto adminLanguageWidget = cpp14::make_unique<AdminLanguageWidget>(viewModels_);
		loginSignal().connect(adminLanguageWidget.get(),&AdminLanguageWidget::login);
		logoutSignal().connect(adminLanguageWidget.get(),&AdminLanguageWidget::logout);
		mainMenu->addItem("Languages",std::move(adminLanguageWidget));

		auto generalSettingsWidget = cpp14::make_unique<AdminGeneralSettingsWidget>(dataStore_);
		loginSignal().connect(generalSettingsWidget.get(),&AdminGeneralSettingsWidget::login);
		logoutSignal().connect(generalSettingsWidget.get(),&AdminGeneralSettingsWidget::logout);
		auto mailSettingsWidget = cpp14::make_unique<AdminMailSettingsWidget>();
		loginSignal().connect(mailSettingsWidget.get(),&AdminMailSettingsWidget::login);
		logoutSignal().connect(mailSettingsWidget.get(),&AdminMailSettingsWidget::logout);
//		auto contactSettingsWidget = cpp14::make_unique<AdminContactSettingsWidget>(dbmodel_);
//		auto footerSettingsWidget = cpp14::make_unique<AdminFooterSettingsWidget>(dbmodel_);

		settingsMenu->addItem("General",std::move(generalSettingsWidget));
		settingsMenu->addItem("Mail",std::move(mailSettingsWidget));
//		settingsMenu->addItem("Contact",std::move(contactSettingsWidget));
//		settingsMenu->addItem("Footer",std::move(footerSettingsWidget));

		auto settingsItem = mainMenu->addItem("Settings");
		settingsItem->setMenu(std::move(settingsMenu));
	} else {
		mustLoginWidget_->setText("You don't have permission to be here. Your presence has been reported.");
		mainWidget_ = mainLayout_->addWidget(cpp14::make_unique<WContainerWidget>(),0);
	}

	loginSignal().emit(login);
}

void AdminWidget::logout() {

	mainLayout_->removeWidget(mainWidget_);
	mustLoginWidget_->setText("You are not logged in.");
	mustLoginWidget_->show();

	logoutSignal().emit();
}

AdminWidget::AdminCategoryWidget::AdminCategoryWidget(const std::shared_ptr<CategoryModel> catmodel) : catmodel_(catmodel) {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout_->setContentsMargins(0,0,0,0);

	auto toolbarWidget = mainLayout_->addWidget(cpp14::make_unique<WContainerWidget>());
	auto toolbarLayout = toolbarWidget->setLayout(cpp14::make_unique<WHBoxLayout>());

	toolbarLayout->addStretch(1);
	auto addButton = toolbarLayout->addWidget(cpp14::make_unique<WImage>("images/add-button.svg"));
	addButton->setHeight(WLength(32));
	addButton->decorationStyle().setCursor(Cursor::PointingHand);
	addButton->setToolTip(WString("Add new category"));
	addButton->clicked().connect( [this] {
		showAddEditDialog();
	});

	treeWidget_ = mainLayout_->addWidget(cpp14::make_unique<WTreeView>(),1);

	treeWidget_->setModel(catmodel_);

	treeWidget_->setRowHeight(26);
	treeWidget_->setHeaderHeight(26);
	treeWidget_->setSortingEnabled(false);
	auto adminActionsDelegate = std::make_shared<AdminCategoryWidget::AdminActionsDelegate>();
	adminActionsDelegate->editCategory().connect(this,&AdminCategoryWidget::showAddEditDialog);
	treeWidget_->setItemDelegateForColumn(3,adminActionsDelegate);
	treeWidget_->addStyleClass("oj-admin-category-tree");
}

void AdminWidget::AdminCategoryWidget::login(Auth::Login& login) {

}

void AdminWidget::AdminCategoryWidget::logout() {

}

void AdminWidget::AdminCategoryWidget::showAddEditDialog(const WModelIndex& index) {

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
	rootCheckBox->checked().connect( [=] {
		selectParent_->setHidden(true);
	} );
	rootCheckBox->unChecked().connect( [=] {
		selectParent_->setHidden(false);
	} );
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

void AdminWidget::AdminCategoryWidget::addEditDialogDone(DialogCode code) {
	int parentId = 0;
	if(code == DialogCode::Accepted) {
		WModelIndex tmpIndex;
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

AdminWidget::AdminCategoryWidget::AdminActionsDelegate::AdminActionsDelegate() {

}

void AdminWidget::AdminCategoryWidget::AdminActionsDelegate::login(Auth::Login& login) {

}

void AdminWidget::AdminCategoryWidget::AdminActionsDelegate::logout() {

}

std::unique_ptr<WWidget> AdminWidget::AdminCategoryWidget::AdminActionsDelegate::update(WWidget *widget, const WModelIndex& index, WFlags<ViewItemRenderFlag> flags) {

	WidgetRef widgetRef(widget);

	bool isNew = false;

	if(widgetRef.w) {
		widgetRef.w = nullptr;
	}

	if(!widgetRef.w) {
		isNew = true;
		widgetRef.created = std::unique_ptr<WWidget>(new WContainerWidget());
		WContainerWidget *t = static_cast<WContainerWidget*>(widgetRef.created.get());
		t->addStyleClass("oj-actions");
		auto layout = t->setLayout(cpp14::make_unique<WHBoxLayout>());
		layout->setContentsMargins(0,4,0,4);
		auto edit = layout->addWidget(cpp14::make_unique<WImage>("images/edit.svg"));
		edit->setHeight(18);
		edit->decorationStyle().setCursor(Cursor::PointingHand);
		edit->setToolTip("Edit category");
		edit->clicked().connect( [=] {
			editCategory_.emit(index.model()->index(index.row(),0,index.parent()));
		});
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

AdminWidget::AdminContestWidget::AdminContestWidget() {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout_->setContentsMargins(0,0,0,0);

	auto toolbarWidget = mainLayout_->addWidget(cpp14::make_unique<WContainerWidget>());
	auto toolbarLayout = toolbarWidget->setLayout(cpp14::make_unique<WHBoxLayout>());

	toolbarLayout->addStretch(1);

	auto addButton = toolbarLayout->addWidget(cpp14::make_unique<WImage>("images/add-button.svg"));
	addButton->setHeight(WLength(32));
	addButton->decorationStyle().setCursor(Cursor::PointingHand);
	addButton->setToolTip(WString("Add new contest"));
}

void AdminWidget::AdminContestWidget::login(Auth::Login& login) {

}

void AdminWidget::AdminContestWidget::logout() {

}

AdminWidget::AdminLanguageWidget::AdminLanguageWidget(ViewModels *viewModels) : viewModels_(viewModels) {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout_->setContentsMargins(0,0,0,0);

	auto toolbarWidget = mainLayout_->addWidget(cpp14::make_unique<WContainerWidget>());
	auto toolbarLayout = toolbarWidget->setLayout(cpp14::make_unique<WHBoxLayout>());

	toolbarLayout->addStretch(1);

	auto addButton = toolbarLayout->addWidget(cpp14::make_unique<WImage>("images/add-button.svg"));
	addButton->setHeight(WLength(32));
	addButton->decorationStyle().setCursor(Cursor::PointingHand);
	addButton->setToolTip(WString("Add new language"));
	        addButton->clicked().connect( [this] {
                showAddEditDialog();
        });

	tableWidget_ = mainLayout_->addWidget(cpp14::make_unique<WTableView>(),1);

	proxyModel_ = std::make_shared<WSortFilterProxyModel>();
	proxyModel_->setSourceModel(viewModels_->getLanguageModel());
	proxyModel_->setDynamicSortFilter(true);

	tableWidget_->setModel(proxyModel_);

	tableWidget_->setRowHeight(26);
	tableWidget_->setHeaderHeight(26);
	tableWidget_->setSortingEnabled(false);
//        tableWidget_->setColumnHidden(3,true);
	//      tableWidget_->setColumnResizeEnabled(false);
	//    tableWidget_->setColumnWidth(1,WLength(695));

	auto adminActionsDelegate = std::make_shared<AdminLanguageWidget::AdminActionsDelegate>();
	adminActionsDelegate->editLanguage().connect(this,&AdminLanguageWidget::showAddEditDialog);
	tableWidget_->setItemDelegateForColumn(3,adminActionsDelegate);
	tableWidget_->addStyleClass("oj-admin-language-table");

}

void AdminWidget::AdminLanguageWidget::login(Auth::Login& login) {

}

void AdminWidget::AdminLanguageWidget::logout() {

}

void AdminWidget::AdminLanguageWidget::showAddEditDialog(const WModelIndex& index) {

	std::string dialogTitle;

	if(!index.isValid()) {
		dialogTitle = "Add new language";
	} else {
		dialogTitle = "Edit language";
	}

	addDialog_ = addChild(cpp14::make_unique<WDialog>(dialogTitle));

	WPushButton *ok = addDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Save"));
	WPushButton *cancel = addDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Cancel"));

	ok->clicked().connect(addDialog_, &WDialog::accept);
	cancel->clicked().connect(addDialog_, &WDialog::reject);

	addDialog_->finished().connect(this,&AdminLanguageWidget::addDialogDone);
	addDialog_->show();

}

void AdminWidget::AdminLanguageWidget::addDialogDone(DialogCode code) {
	if(code == DialogCode::Accepted) {
	}

	removeChild(addDialog_);
}

AdminWidget::AdminLanguageWidget::AdminActionsDelegate::AdminActionsDelegate() {

}

void AdminWidget::AdminLanguageWidget::AdminActionsDelegate::login(Auth::Login& login) {

}

void AdminWidget::AdminLanguageWidget::AdminActionsDelegate::logout() {

}

std::unique_ptr<WWidget> AdminWidget::AdminLanguageWidget::AdminActionsDelegate::update(WWidget *widget, const WModelIndex& index, WFlags<ViewItemRenderFlag> flags) {

	WidgetRef widgetRef(widget);

	bool isNew = false;

	if(widgetRef.w) {
		widgetRef.w = nullptr;
	}

	if(!widgetRef.w) {
		isNew = true;
		widgetRef.created = std::unique_ptr<WWidget>(new WContainerWidget());
		WContainerWidget *t = static_cast<WContainerWidget*>(widgetRef.created.get());
		t->addStyleClass("oj-actions");
		auto layout = t->setLayout(cpp14::make_unique<WHBoxLayout>());
		layout->setContentsMargins(0,4,0,4);
		auto edit = layout->addWidget(cpp14::make_unique<WImage>("images/edit.svg"));
		edit->setHeight(18);
		edit->decorationStyle().setCursor(Cursor::PointingHand);
		edit->setToolTip("Edit language");
		edit->clicked().connect( [=] {
			editLanguage_.emit(index.model()->index(index.row(),0,index.parent()));
		});
		auto trash = layout->addWidget(cpp14::make_unique<WImage>("images/trash.svg"));
		trash->setHeight(18);
		trash->decorationStyle().setCursor(Cursor::PointingHand);
		trash->setToolTip("Delete language");
	}

	if(isNew) {
		return std::move(widgetRef.created);
	} else {
		return nullptr;
	}

}

AdminWidget::AdminProblemWidget::AdminProblemWidget(ViewModels *viewModels, DBModel *dbmodel) : viewModels_(viewModels), dbmodel_(dbmodel) {

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
	addButton->clicked().connect( [this] {
		showAddEditDialog();
	});

	tableWidget_ = mainLayout_->addWidget(cpp14::make_unique<WTableView>(),1);

	proxyModel_ = std::make_shared<WSortFilterProxyModel>();
	proxyModel_->setSourceModel(viewModels_->getProblemModel());
	proxyModel_->setDynamicSortFilter(true);

	tableWidget_->setModel(proxyModel_);

	tableWidget_->setRowHeight(26);
	tableWidget_->setHeaderHeight(26);
	tableWidget_->setSortingEnabled(false);
	tableWidget_->setColumnHidden(3,true);
	tableWidget_->setColumnResizeEnabled(false);
	tableWidget_->setColumnWidth(1,WLength(695));

	auto adminActionsDelegate = std::make_shared<AdminProblemWidget::AdminActionsDelegate>();
	adminActionsDelegate->editProblem().connect(this,&AdminProblemWidget::showAddEditDialog);
	tableWidget_->setItemDelegateForColumn(2,adminActionsDelegate);
	tableWidget_->addStyleClass("oj-admin-problem-table");
}

void AdminWidget::AdminProblemWidget::login(Auth::Login& login) {

}

void AdminWidget::AdminProblemWidget::logout() {

}

void AdminWidget::AdminProblemWidget::problemSelectorSlot() {

	proxyModel_->setFilterKeyColumn(0);
	proxyModel_->setFilterRole(ItemDataRole::Display);
	if(problemSelector_->text().toUTF8() == "") {
		proxyModel_->setFilterRegExp(std::make_unique<std::regex>(".*"));
	} else {
		proxyModel_->setFilterRegExp(std::make_unique<std::regex>(problemSelector_->text().toUTF8()));
	}
}

void AdminWidget::AdminProblemWidget::showAddEditDialog(const WModelIndex& index) {

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

	auto htmlDescription = cpp14::make_unique<WFileUpload>();
	htmlDescription_ = htmlDescription.get();
	htmlDescription_->changed().connect(htmlDescription_, &WFileUpload::upload);

	auto htmlProgress = cpp14::make_unique<WProgressBar>();
	WProgressBar *htmlProgress_ = htmlProgress.get();
	htmlDescription_->setProgressBar(htmlProgress_);

	auto pdfDescription = cpp14::make_unique<WFileUpload>();
	pdfDescription_ = pdfDescription.get();
	pdfDescription_->changed().connect(pdfDescription_, &WFileUpload::upload);

	auto pdfProgress = cpp14::make_unique<WProgressBar>();
	WProgressBar *pdfProgress_ = pdfProgress.get();
	pdfDescription_->setProgressBar(pdfProgress_);

	auto categories = cpp14::make_unique<WTreeView>();
	categories->setModel(viewModels_->getCategoryModel());
	categories->setColumnHidden(1,true);
	categories->setColumnHidden(2,true);
	categories->setColumnHidden(3,true);
	categories->setSelectionMode(SelectionMode::Extended);
	categories_ = categories.get();

	result->bindString("idlabel",WString("ID"));
	result->bindString("titlelabel",WString("Title"));
	result->bindString("htmldescriptionlabel",WString("HTML problem description"));
	result->bindString("pdfdescriptionlabel",WString("PDF problem description"));
	result->bindString("categorieslabel",WString("Categories"));
	result->bindWidget("id",std::move(id));
	result->bindWidget("title",std::move(title));
	result->bindWidget("htmldescription",std::move(htmlDescription));
	result->bindWidget("htmlprogress",std::move(htmlProgress));
	result->bindWidget("pdfdescription",std::move(pdfDescription));
	result->bindWidget("pdfprogress",std::move(pdfProgress));
	result->bindWidget("categories",std::move(categories));

	WPushButton *ok = addDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Save"));
	WPushButton *cancel = addDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Cancel"));

	ok->clicked().connect(addDialog_, &WDialog::accept);
	cancel->clicked().connect(addDialog_, &WDialog::reject);

/*      ok->disable();
        description_->uploaded().connect(ok, &WPushButton::enable);*/

	if(index.isValid()) {
		const std::shared_ptr<ProblemModel> problemModel = viewModels_->getProblemModel();
		long long problemId = cpp17::any_cast<long long>(problemModel->data(index));
		id_->setText(std::to_string(problemId));
		title_->setText(cpp17::any_cast<std::string>(problemModel->data(problemModel->index(index.row(),1))));
		categories_->setSelectedIndexes(viewModels_->getProblemModel()->getCategories(problemId));
	}

	addDialog_->finished().connect(this,&AdminProblemWidget::addDialogDone);
	addDialog_->show();

}

void AdminWidget::AdminProblemWidget::addDialogDone(DialogCode code) {
	if(code == DialogCode::Accepted) {
		viewModels_->getProblemModel()->addProblem(std::stoi(id_->text().toUTF8()),title_->text().toUTF8());
		viewModels_->getProblemModel()->setCategories(std::stoi(id_->text().toUTF8()),categories_->selectedIndexes());
		std::ifstream htmlFile(htmlDescription_->spoolFileName(), std::ios::binary);
		std::string htmlFileContents(std::istreambuf_iterator<char>{htmlFile},{});
		std::ifstream pdfFile(pdfDescription_->spoolFileName(), std::ios::binary);
		std::vector<unsigned char> pdfFileContents(std::istreambuf_iterator<char>{pdfFile},{});
		dbmodel_->updateDescription(std::stoi(id_->text().toUTF8()),std::optional<std::string>{htmlFileContents},std::optional<std::vector<unsigned char> >{pdfFileContents});
	}

	removeChild(addDialog_);
}

AdminWidget::AdminProblemWidget::AdminActionsDelegate::AdminActionsDelegate() {

}

void AdminWidget::AdminProblemWidget::AdminActionsDelegate::login(Auth::Login& login) {

}

void AdminWidget::AdminProblemWidget::AdminActionsDelegate::logout() {

}

std::unique_ptr<WWidget> AdminWidget::AdminProblemWidget::AdminActionsDelegate::update(WWidget *widget, const WModelIndex& index, WFlags<ViewItemRenderFlag> flags) {

	WidgetRef widgetRef(widget);

	bool isNew = false;

	if(widgetRef.w) {
		widgetRef.w = nullptr;
	}

	if(!widgetRef.w) {
		isNew = true;
		widgetRef.created = std::unique_ptr<WWidget>(new WContainerWidget());
		WContainerWidget *t = static_cast<WContainerWidget*>(widgetRef.created.get());
		t->addStyleClass("oj-actions");
		auto layout = t->setLayout(cpp14::make_unique<WHBoxLayout>());
		layout->setContentsMargins(0,4,0,4);
		auto edit = layout->addWidget(cpp14::make_unique<WImage>("images/edit.svg"));
		edit->setHeight(18);
		edit->decorationStyle().setCursor(Cursor::PointingHand);
		edit->setToolTip("Edit problem");
		edit->clicked().connect( [=] {
			editProblem_.emit(index.model()->index(index.row(),0,index.parent()));
		});
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

AdminWidget::AdminGeneralSettingsWidget::AdminGeneralSettingsWidget(DataStore *dataStore) : dataStore_(dataStore) {

	setTemplateText(WString::tr("admin-settings-general"));

	addFunction("block",&WTemplate::Functions::block);
	addFunction("tr",&WTemplate::Functions::tr);
	addFunction("id",&WTemplate::Functions::id);

	auto siteTitle = cpp14::make_unique<WLineEdit>();
	siteTitle->changed().connect( [=] {
		siteTitleChanged_ = true;
	});
	siteTitle_ = bindWidget("sitetitle-setting",std::move(siteTitle));

	auto siteLogo = cpp14::make_unique<WLineEdit>();
	siteLogo->changed().connect( [=] {
		siteLogoChanged_ = true;
	});
	siteLogo_ = bindWidget("sitelogo-setting",std::move(siteLogo));

	auto siteColor = cpp14::make_unique<OJColorPicker>();
	siteColor->setWidth(50);
	siteColor->colorInput().connect( [=] {
		siteColorChanged_ = true;
	});
	siteColor_ = bindWidget("sitecolor-setting",std::move(siteColor));

	auto googleAnalytics = cpp14::make_unique<WLineEdit>();
	googleAnalytics->changed().connect( [=] {
		googleAnalyticsChanged_ = true;
	});
	googleAnalytics_ = bindWidget("googleanalytics-setting",std::move(googleAnalytics));

	auto applyButton = cpp14::make_unique<WPushButton>("Apply");
	applyButton->addStyleClass("btn-primary");
	applyButton->clicked().connect(this,&AdminWidget::AdminGeneralSettingsWidget::applyClicked);
	bindWidget("apply-button",std::move(applyButton));

	auto resetButton = cpp14::make_unique<WPushButton>("Reset");
	resetButton->clicked().connect(this,&AdminWidget::AdminGeneralSettingsWidget::resetClicked);
	bindWidget("reset-button",std::move(resetButton));
}

void AdminWidget::AdminGeneralSettingsWidget::login(Auth::Login& login) {

	login_ = &login;
	reset();
}

void AdminWidget::AdminGeneralSettingsWidget::logout() {

}

void AdminWidget::AdminGeneralSettingsWidget::reset() {

	siteTitle_->setText(dataStore_->getSettingStore()->getSetting("sitetitle"));
	siteLogo_->setText(dataStore_->getSettingStore()->getSetting("sitelogo"));
	siteColor_->setColor(WColor(dataStore_->getSettingStore()->getSetting("sitecolor")));
	googleAnalytics_->setText(dataStore_->getSettingStore()->getSetting("googleanalytics"));

	siteTitleChanged_ = false;
	siteLogoChanged_ = false;
	siteColorChanged_ = false;
	googleAnalyticsChanged_ = false;
}

void AdminWidget::AdminGeneralSettingsWidget::resetClicked() {

	auto warningBox = addChild(cpp14::make_unique<WMessageBox>("Are you sure?","All changes will be lost. Do you want to continue?",
	                                                           Icon::Warning,StandardButton::Yes | StandardButton::No));
	warningBox->buttonClicked().connect( [=] (StandardButton button) {
		switch(button) {
		case StandardButton::Yes:
			reset();
			break;
		case StandardButton::No:
			break;
		}
		removeChild(warningBox);
	});
	warningBox->show();
}

void AdminWidget::AdminGeneralSettingsWidget::applyClicked() {

	if(!siteTitleChanged_ && !siteLogoChanged_ && !siteColorChanged_ && !googleAnalyticsChanged_) return;

	WStringStream strm;

	strm << "The following data will be updated:<br/><br/>";
	strm << "<ul>";
	if(siteTitleChanged_) strm << "<li>Site title to: <b>" << siteTitle_->text().toUTF8() << "</b></li>";
	if(siteLogoChanged_) strm << "<li>Site logo to: <b>" << siteLogo_->text().toUTF8() << "</b></li>";
	if(siteColorChanged_) strm << "<li>Site color to: <b>" << siteColor_->color().cssText() << "</b></li>";
	if(googleAnalyticsChanged_) strm << "<li>Google Analytics to: <b>" << googleAnalytics_->text().toUTF8() << "</b></li>";
	strm << "</ul>";
	strm << "<br/>Do you want to continue?";

	auto warningBox = addChild(cpp14::make_unique<WMessageBox>("Are you sure?","",Icon::Information,StandardButton::Yes | StandardButton::No));
	warningBox->textWidget()->setTextFormat(TextFormat::XHTML);
	warningBox->setText(strm.str());

	warningBox->buttonClicked().connect( [=] (StandardButton button) {
		switch(button) {
		case StandardButton::Yes:
			{
			        if(siteTitleChanged_) dataStore_->getSettingStore()->setSetting("sitetitle", siteTitle_->text().toUTF8());
			        if(siteLogoChanged_) dataStore_->getSettingStore()->setSetting("sitelogo", siteLogo_->text().toUTF8());
			        if(siteColorChanged_) dataStore_->getSettingStore()->setSetting("sitecolor",siteColor_->color().cssText());
			        if(googleAnalyticsChanged_) dataStore_->getSettingStore()->setSetting("googleanalytics",googleAnalytics_->text().toUTF8());
			}
			break;
		case StandardButton::No:
			break;
		}
		removeChild(warningBox);
	});
	warningBox->show();
}

AdminWidget::AdminMailSettingsWidget::AdminMailSettingsWidget() {

	auto app = WApplication::instance();

	setTemplateText(WString::tr("admin-settings-mail"));

	addFunction("block",&WTemplate::Functions::block);
	addFunction("tr",&WTemplate::Functions::tr);
	addFunction("id",&WTemplate::Functions::id);

	std::vector<std::string> settingFields = {
		"smtp-self-host",
		"smtp-host",
		"smtp-port",
		"smtp-auth-username",
		"smtp-auth-method",
		"smtp-transport-encryption",
		"auth-mail-sender-name",
		"auth-mail-sender-address"
	};

	std::string tmpstr;
	for(const auto &field: settingFields) {
		auto tmpLineEdit = cpp14::make_unique<WLineEdit>(app->readConfigurationProperty(field,tmpstr) ? tmpstr : "<not set>");
		tmpLineEdit->disable();
		bindWidget(field + "-setting",std::move(tmpLineEdit));
	}
	auto tmpLineEdit = cpp14::make_unique<WLineEdit>(app->readConfigurationProperty("smtp-auth-password",tmpstr) ? "<set and hidden>" : "<not set>");
	tmpLineEdit->disable();
	bindWidget("smtp-auth-password-setting",std::move(tmpLineEdit));

	bindWidget("config-file-path",cpp14::make_unique<WText>(WT_CONFIG_XML));

	auto testEmail = cpp14::make_unique<WLineEdit>();
	testEmail->setPlaceholderText("test@address.here");
	auto testEmailPtr = testEmail.get();
	bindWidget("test-mail",std::move(testEmail));

	auto testButton = cpp14::make_unique<WPushButton>("Send");
	testButton->clicked().connect( [=] {
		testEmailPtr->removeStyleClass("has-error");
		testEmailPtr->removeStyleClass("has-success");
		Mail::Message message;
		std::string tmpstr1;
		std::string tmpstr2;
		app->readConfigurationProperty("auth-mail-sender-address",tmpstr1);
		app->readConfigurationProperty("auth-mail-sender-name",tmpstr2);
		message.setFrom(Mail::Mailbox(tmpstr1,tmpstr2));
		message.addRecipient(Mail::RecipientType::To,Mail::Mailbox(testEmailPtr->text().toUTF8(),testEmailPtr->text().toUTF8()));
		message.setSubject("Test email");
		message.setBody("Mail configuration seems to work!");

		Mail::Client client;
		client.connect();
		bool result = client.send(message);

		testEmailPtr->addStyleClass(result ? "has-success" : "has-error");
	});

	bindWidget("test-mail-button",std::move(testButton));
}

void AdminWidget::AdminMailSettingsWidget::login(Auth::Login& login) {

}

void AdminWidget::AdminMailSettingsWidget::logout() {

}

AdminWidget::AdminUserWidget::AdminUserWidget(ViewModels *viewModels, DBModel *dbmodel) : viewModels_(viewModels), dbmodel_(dbmodel) {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout_->setContentsMargins(0,0,0,0);

	auto toolbarWidget = mainLayout_->addWidget(cpp14::make_unique<WContainerWidget>());
	auto toolbarLayout = toolbarWidget->setLayout(cpp14::make_unique<WHBoxLayout>());

	auto userSelectorWidget = toolbarLayout->addWidget(cpp14::make_unique<WTemplate>(WString::tr("lineEdit-template")),1);
	userSelectorWidget->addFunction("id",&WTemplate::Functions::id);
	auto userSelector = cpp14::make_unique<WLineEdit>();
	userSelector_ = userSelector.get();
	userSelectorWidget->bindString("label","Search user");
	userSelectorWidget->bindWidget("edit",std::move(userSelector));

	//      problemSelector_->setValidator(std::make_shared<Wt::WIntValidator>());
	//     problemSelector_->textInput().connect(this,&AdminProblemWidget::problemSelectorSlot);

	tableWidget_ = mainLayout_->addWidget(cpp14::make_unique<WTableView>(),1);

	proxyModel_ = std::make_shared<WSortFilterProxyModel>();
	proxyModel_->setSourceModel(viewModels_->getUserModel());
	proxyModel_->setDynamicSortFilter(true);

	tableWidget_->setModel(proxyModel_);

	tableWidget_->setRowHeight(26);
	tableWidget_->setHeaderHeight(26);
	tableWidget_->setSortingEnabled(false);
	tableWidget_->setColumnResizeEnabled(false);

	auto adminActionsDelegate = std::make_shared<AdminUserWidget::AdminActionsDelegate>();
	adminActionsDelegate->editUser().connect(this,&AdminUserWidget::showEditDialog);
	tableWidget_->setItemDelegateForColumn(5,adminActionsDelegate);
	tableWidget_->addStyleClass("oj-admin-user-table");
}

void AdminWidget::AdminUserWidget::login(Auth::Login& login) {

}

void AdminWidget::AdminUserWidget::logout() {

}

void AdminWidget::AdminUserWidget::showEditDialog(const WModelIndex& index) {

	editDialog_ = addChild(cpp14::make_unique<WDialog>("Edit user"));

	auto result = editDialog_->contents()->addWidget(cpp14::make_unique<WTemplate>(WString::tr("editUser-template")));
	result->addFunction("id",&WTemplate::Functions::id);
	result->addFunction("block",&WTemplate::Functions::block);
	result->addFunction("tr",&WTemplate::Functions::tr);

	auto id = result->bindWidget("id-setting",cpp14::make_unique<WLineEdit>());
	id->disable();

	auto username = result->bindWidget("username-setting",cpp14::make_unique<WLineEdit>());
	username->disable();

	auto firstname = result->bindWidget("firstname-setting",cpp14::make_unique<WLineEdit>());
	auto lastname = result->bindWidget("lastname-setting",cpp14::make_unique<WLineEdit>());
	auto email = result->bindWidget("email-setting",cpp14::make_unique<WLineEdit>());
	auto password = result->bindWidget("password-setting",cpp14::make_unique<WCheckBox>("Reset password"));
	auto role = result->bindWidget("role-setting",cpp14::make_unique<WComboBox>());
	auto status = result->bindWidget("status-setting",cpp14::make_unique<WComboBox>());

	WPushButton *ok = editDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Save"));
	WPushButton *cancel = editDialog_->footer()->addWidget(cpp14::make_unique<WPushButton>("Cancel"));

	ok->clicked().connect(editDialog_, &WDialog::accept);
	cancel->clicked().connect(editDialog_, &WDialog::reject);

/*      ok->disable();
        description_->uploaded().connect(ok, &WPushButton::enable);*/

	editDialog_->finished().connect(this,&AdminUserWidget::editDialogDone);
	editDialog_->show();

}

void AdminWidget::AdminUserWidget::editDialogDone(DialogCode code) {
	if(code == DialogCode::Accepted) {
	}

	removeChild(editDialog_);
}

AdminWidget::AdminUserWidget::AdminActionsDelegate::AdminActionsDelegate() {

}

void AdminWidget::AdminUserWidget::AdminActionsDelegate::login(Auth::Login& login) {

}

void AdminWidget::AdminUserWidget::AdminActionsDelegate::logout() {

}

std::unique_ptr<WWidget> AdminWidget::AdminUserWidget::AdminActionsDelegate::update(WWidget *widget, const WModelIndex& index, WFlags<ViewItemRenderFlag> flags) {

	WidgetRef widgetRef(widget);

	bool isNew = false;

	if(widgetRef.w) {
		widgetRef.w = nullptr;
	}

	if(!widgetRef.w) {
		isNew = true;
		widgetRef.created = std::unique_ptr<WWidget>(new WContainerWidget());
		WContainerWidget *t = static_cast<WContainerWidget*>(widgetRef.created.get());
		t->addStyleClass("oj-actions");
		auto layout = t->setLayout(cpp14::make_unique<WHBoxLayout>());
		layout->setContentsMargins(0,4,0,4);
		auto edit = layout->addWidget(cpp14::make_unique<WImage>("images/edit.svg"));
		edit->setHeight(18);
		edit->decorationStyle().setCursor(Cursor::PointingHand);
		edit->setToolTip("Edit user");
		edit->clicked().connect( [=] {
			editUser_.emit(index.model()->index(index.row(),0,index.parent()));
		});
	}

	if(isNew) {
		return std::move(widgetRef.created);
	} else {
		return nullptr;
	}

}
