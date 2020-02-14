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
#include "dbmodel/DBModel.h"
#include "AdminSettingsWidget.h"

using namespace Wt;

AdminSettingsWidget::AdminSettingsWidget(DBModel *dbmodel) : dbmodel_(dbmodel) {

	Settings settings = dbmodel_->getSettings();

	Dbo::Transaction transaction = dbmodel_->startTransaction();

	for(Settings::const_iterator i = settings.begin(); i != settings.end(); i++) {
		dbo::ptr<Setting> setting = *i;
		auto result = addWidget(cpp14::make_unique<WTemplate>(WString::tr("lineEdit-template")));
		result->addFunction("id",&WTemplate::Functions::id);

		auto edit = cpp14::make_unique<WLineEdit>(setting->settingValue);

		result->bindString("label",WString::tr(setting->settingName));
		result->bindWidget("edit", std::move(edit));
	}
}
