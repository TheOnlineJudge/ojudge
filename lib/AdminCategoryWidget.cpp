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
#include <Wt/WLineEdit.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>
#include "AdminCategoryWidget.h"

using namespace Wt;

AdminCategoryWidget::AdminCategoryWidget(const std::shared_ptr<CategoryModel> catmodel) {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
        mainLayout_->setContentsMargins(0,0,0,0);

	treeWidget_ = mainLayout_->addWidget(cpp14::make_unique<WTreeView>()) ;

	treeWidget_->setModel(catmodel);

	treeWidget_->setRowHeight(26) ;
	treeWidget_->setHeaderHeight(26) ;
	treeWidget_->setSortingEnabled(false) ;
	

/*	auto result = mainLayout->addWidget(cpp14::make_unique<WTemplate>(WString::tr("lineEdit-template")));
	result->addFunction("id", &WTemplate::Functions::id);

	auto button = mainLayout->addWidget(cpp14::make_unique<WPushButton>("Create"));

	auto edit = Wt::cpp14::make_unique<Wt::WLineEdit>();
	
	result->bindString("label", "Title");
	result->bindWidget("edit", std::move(edit)) ;*/

}

/*
void AdminCategoryWidget::populateCategory(int parent, WTreeTableNode *root) {

	Categories categories = dbmodel_->getCategories(parent) ;
	Dbo::Transaction transaction = dbmodel_->startTransaction();

	for(Categories::const_iterator i = categories.begin(); i != categories.end(); i++) {
		dbo::ptr<Category> category = *i;
		auto node = std::make_unique<WTreeTableNode>(std::string(category->title)) ;
		node->setColumnWidget(1,std::make_unique<WText>("0")) ;
		node->setColumnWidget(2,std::make_unique<WText>("0")) ;
		root->addChildNode(std::move(node)) ;
		
//		mainLayout_->addWidget(std::move(cpp14::make_unique<AdminCategoryCategoryWidget>(std::string(category->title),(category->children.size() ? true : false)))) ;
	}
}
*/
