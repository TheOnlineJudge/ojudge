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
#include "SubmissionsWidget.h"

using namespace Wt;

SubmissionsWidget::SubmissionsWidget() {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	auto pageTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Submissions"),0);
	pageTitle->addStyleClass("pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

}
