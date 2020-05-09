/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "../dbmodel/DBModel.h"

#include "DataStore.h"
#include "CategoryStore.h"
#include "CountryStore.h"
#include "LanguageStore.h"
#include "ProblemStore.h"
#include "SettingStore.h"
#include "StatisticsStore.h"
#include "SubmissionStore.h"
#include "UserStore.h"
#include "VerdictResultStore.h"

DataStore::DataStore(DBModel *dbModel) : dbModel_(dbModel) {

	categoryStore_ = new CategoryStore(dbModel_);
	countryStore_ = new CountryStore();
	problemStore_ = new ProblemStore(dbModel_);
	languageStore_ = new LanguageStore(dbModel_);
	settingStore_ = new SettingStore(dbModel_);
	statisticsStore_ = new StatisticsStore(dbModel_);
	submissionStore_ = new SubmissionStore(dbModel_);
	userStore_ = new UserStore(dbModel_);
	verdictResultStore_ = new VerdictResultStore(dbModel_);

}

CategoryStore *DataStore::getCategoryStore() {
	return categoryStore_;
}

CountryStore *DataStore::getCountryStore() {
	return countryStore_;
}

ProblemStore *DataStore::getProblemStore() {
	return problemStore_;
}

LanguageStore *DataStore::getLanguageStore() {
	return languageStore_;
}

UserStore *DataStore::getUserStore() {
	return userStore_;
}

SettingStore *DataStore::getSettingStore() {
	return settingStore_;
}

StatisticsStore *DataStore::getStatisticsStore() {
	return statisticsStore_;
}

SubmissionStore *DataStore::getSubmissionStore() {
	return submissionStore_;
}

VerdictResultStore *DataStore::getVerdictResultStore() {
	return verdictResultStore_;
}
