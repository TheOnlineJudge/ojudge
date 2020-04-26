/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <mutex>
#include "../ojudgeApp.h"
#include "SettingStore.h"

std::mutex setSetting_mutex;

using namespace Wt;

SettingStore::SettingStore(DBModel *dbModel) : dbModel_(dbModel) {

	Settings settings = dbModel_->getSiteSettings();
	dbo::Transaction t = dbModel_->startTransaction();

	for(const auto &setting: settings) {
		settingData_[setting->settingName] = setting->settingValue;
	}
}

const std::string SettingStore::getSetting(std::string setting) {
	return settingData_.at(setting);
}

bool SettingStore::setSetting(std::string setting, std::string value) {
	if(settingData_.find(setting) == settingData_.end()) {
		return false;
	} else {
		std::lock_guard<std::mutex> guard(setSetting_mutex);
		settingData_[setting] = value;
		dbModel_->updateSiteSetting(setting,value);
	}
	return true;
}
