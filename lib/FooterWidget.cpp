/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WText.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WAnchor.h>
#include <string>
#include <sstream>
#include "../version.h"
#include "dbmodel/DBModel.h"
#include "FooterWidget.h"

using namespace Wt;

FooterWidget::FooterWidget(DBModel *dbmodel) : dbmodel_(dbmodel) {

	setPadding(WLength(5));
	addStyleClass("oj-footer");

	auto wrapWidget = addWidget(cpp14::make_unique<WContainerWidget>());
	wrapWidget->setMargin(WLength(0));
	wrapWidget->setPadding(WLength(0));

	auto mainLayout = wrapWidget->setLayout(cpp14::make_unique<WHBoxLayout>());

	auto aboutWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
	auto aboutLayout = aboutWidget->setLayout(cpp14::make_unique<WVBoxLayout>());
	aboutLayout->setContentsMargins(0,0,0,0);
	auto aboutTitle = aboutLayout->addWidget(cpp14::make_unique<WText>(dbmodel_->getSetting("siteTitle")));
	aboutTitle->addStyleClass(WString("oj-footer-head"));

	aboutLayout->addWidget(cpp14::make_unique<WAnchor>(WLink(LinkType::InternalPath,"/about"),"About"));
	aboutLayout->addWidget(cpp14::make_unique<WAnchor>(WLink(LinkType::InternalPath,"/team"),"Our Team"));
	aboutLayout->addWidget(cpp14::make_unique<WAnchor>(WLink(LinkType::InternalPath,"/facts"),"Interesting Facts"));
	aboutLayout->addStretch(1);

	auto contributeWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
	auto contributeLayout = contributeWidget->setLayout(cpp14::make_unique<WVBoxLayout>());
	contributeLayout->setContentsMargins(0,0,0,0);
	auto contributeTitle = contributeLayout->addWidget(cpp14::make_unique<WText>("Contribute"));
	contributeTitle->addStyleClass(WString("oj-footer-head"));

	contributeLayout->addWidget(cpp14::make_unique<WAnchor>(WLink(LinkType::InternalPath,"/contribute"),"Contribute"));
	contributeLayout->addWidget(cpp14::make_unique<WAnchor>(WLink(LinkType::InternalPath,"/sponsors"),"Our Sponsors"));
	contributeLayout->addStretch(1);

	auto helpWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
	auto helpLayout = helpWidget->setLayout(cpp14::make_unique<WVBoxLayout>());
	helpLayout->setContentsMargins(0,0,0,0);
	auto helpTitle = helpLayout->addWidget(cpp14::make_unique<WText>("Help"));
	helpTitle->addStyleClass(WString("oj-footer-head"));

	helpLayout->addWidget(cpp14::make_unique<WAnchor>(WLink(LinkType::InternalPath,"/tutorial"),"Getting Started"));
	helpLayout->addWidget(cpp14::make_unique<WAnchor>(WLink(LinkType::InternalPath,"/languages"),"Language Templates"));
	helpLayout->addStretch(1);

	auto contactusWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
	auto contactusLayout = contactusWidget->setLayout(cpp14::make_unique<WVBoxLayout>());
	contactusLayout->setContentsMargins(0,0,0,0);
	auto contactusTitle = contactusLayout->addWidget(cpp14::make_unique<WText>("Contact Us"));
	contactusTitle->addStyleClass(WString("oj-footer-head"));

	contactusLayout->addWidget(cpp14::make_unique<WAnchor>(WLink(LinkType::InternalPath,"/contact"),"Contact Form"));
	contactusLayout->addStretch(1);


	std::stringstream footerMsg;
	footerMsg << "Using OJudge version " << OJUDGE_VERSION_MAJOR <<
	        "." << OJUDGE_VERSION_MINOR <<
	        "." << OJUDGE_VERSION_RELEASE <<
	        " - Compile: " << OJUDGE_VERSION_COMPILE;

	auto bottomWrap = addWidget(cpp14::make_unique<WContainerWidget>());
	bottomWrap->setMargin(WLength(0));
	bottomWrap->setPadding(WLength(0));

	auto bottomLayout = bottomWrap->setLayout(cpp14::make_unique<WHBoxLayout>());
	bottomLayout->setContentsMargins(0,0,0,0);
	bottomLayout->addStretch(1);

	auto versionText = bottomLayout->addWidget(cpp14::make_unique<WText>(footerMsg.str()),0);
	versionText->decorationStyle().setForegroundColor(WColor("white"));

}

void FooterWidget::login(Auth::Login& login) {

}

void FooterWidget::logout() {

}
