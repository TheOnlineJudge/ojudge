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
#include "AdminProblemWidget.h"

using namespace Wt;

AdminProblemWidget::AdminProblemWidget(const std::shared_ptr<ProblemModel> probmodel) {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
        mainLayout_->setContentsMargins(0,0,0,0);

	tableWidget_ = mainLayout_->addWidget(cpp14::make_unique<WTableView>()) ;

	proxyModel_ = std::make_shared<WSortFilterProxyModel>();
	proxyModel_->setSourceModel(probmodel);

	tableWidget_->setModel(proxyModel_);

	tableWidget_->setRowHeight(26) ;
	tableWidget_->setHeaderHeight(26) ;
	tableWidget_->setSortingEnabled(false) ;
}
