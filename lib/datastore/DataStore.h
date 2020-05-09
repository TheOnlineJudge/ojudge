/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef DATASTORE_H
#define DATASTORE_H

class DBModel;

class CategoryStore;
class CountryStore;
class ProblemStore;
class LanguageStore;
class SettingStore;
class StatisticsStore;
class SubmissionStore;
class UserStore;
class VerdictResultStore;

class DataStore {
public:
DataStore(DBModel *dbModel);
CategoryStore *getCategoryStore();
CountryStore *getCountryStore();
ProblemStore *getProblemStore();
LanguageStore *getLanguageStore();
SettingStore *getSettingStore();
StatisticsStore *getStatisticsStore();
SubmissionStore *getSubmissionStore();
UserStore *getUserStore();
VerdictResultStore *getVerdictResultStore();

private:
DBModel *dbModel_;
CategoryStore *categoryStore_;
CountryStore *countryStore_;
ProblemStore *problemStore_;
LanguageStore *languageStore_;
SettingStore *settingStore_;
StatisticsStore *statisticsStore_;
SubmissionStore *submissionStore_;
UserStore *userStore_;
VerdictResultStore *verdictResultStore_;
};

#endif // DATASTORE_H
