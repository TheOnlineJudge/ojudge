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
#include <Wt/WTable.h>
#include "SubmissionsWidget.h"

using namespace Wt;

SubmissionsWidget::SubmissionsWidget() {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	auto pageTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Submissions"),0);
	pageTitle->addStyleClass("oj-pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);

	auto submissionTable = mainLayout->addWidget(cpp14::make_unique<WTable>(),2);
	submissionTable->setHeaderCount(1);

	
	submissionTable->elementAt(0,1)->addNew<WText>("ID");
	submissionTable->elementAt(0,2)->addNew<WText>("version");
	submissionTable->elementAt(0,3)->addNew<WText>("problem");
	submissionTable->elementAt(0,4)->addNew<WText>("user");
	submissionTable->elementAt(0,5)->addNew<WText>("testcase");
	submissionTable->elementAt(0,6)->addNew<WText>("contest");
	submissionTable->elementAt(0,7)->addNew<WText>("time");
	submissionTable->elementAt(0,8)->addNew<WText>("language");
	
}

void SubmissionsWidget::login(Auth::Login& login) {

}

void SubmissionsWidget::logout() {

}
