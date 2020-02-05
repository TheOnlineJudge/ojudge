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
}
