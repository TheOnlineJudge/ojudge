/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WLineEdit.h>
#include <Wt/WTemplate.h>
#include <Wt/WMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WContainerWidget.h>
#include "dbmodel/DBModel.h"
#include "AdminSettingsWidget.h"

using namespace Wt;

AdminSettingsWidget::AdminSettingsWidget(DBModel *dbmodel) : dbmodel_(dbmodel) {

	Settings settings = dbmodel_->getSettings();
	Dbo::Transaction transaction = dbmodel_->startTransaction();

	auto mainLayout = setLayout(cpp14::make_unique<WHBoxLayout>());

	auto mainStack = mainLayout->addWidget(cpp14::make_unique<WStackedWidget>(),1);
	auto leftMenu = mainLayout->insertWidget(0,cpp14::make_unique<WMenu>(mainStack),0);
	leftMenu->addStyleClass("flex-column");
	leftMenu->setWidth(200);

	auto generalSettings = cpp14::make_unique<WContainerWidget>();

	for(Settings::const_iterator i = settings.begin(); i != settings.end(); i++) {
		dbo::ptr<Setting> setting = *i;
		auto result = generalSettings->addWidget(cpp14::make_unique<WTemplate>(WString::tr("lineEdit-template")));
		result->addFunction("id",&WTemplate::Functions::id);

		auto edit = cpp14::make_unique<WLineEdit>(setting->settingValue);

		result->bindString("label",WString::tr(setting->settingName));
		result->bindWidget("edit", std::move(edit));
	}

	auto generalSettingsMenu = leftMenu->addItem("General",std::move(generalSettings));
	auto mailSettingsMenu = leftMenu->addItem("Mail",cpp14::make_unique<WContainerWidget>());
	auto contactSettingsMenu = leftMenu->addItem("Contact",cpp14::make_unique<WContainerWidget>());
	auto footerSettingsMenu = leftMenu->addItem("Footer",cpp14::make_unique<WContainerWidget>());
}
