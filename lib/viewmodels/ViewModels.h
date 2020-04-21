/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef VIEWMODELS_H
#define VIEWMODELS_H

#include "CategoryModel.h"
#include "CountryModel.h"
#include "LanguageModel.h"
#include "ProblemModel.h"
#include "UserModel.h"

class DataStore;

class ViewModels {
public:
ViewModels(DataStore *dataStore);
const std::shared_ptr<CategoryModel> getCategoryModel();
const std::shared_ptr<CountryModel> getCountryModel();
const std::shared_ptr<LanguageModel> getLanguageModel();
const std::shared_ptr<ProblemModel> getProblemModel();
const std::shared_ptr<UserModel> getUserModel();

private:
DataStore *dataStore_;
std::shared_ptr<CategoryModel> categoryModel_;
std::shared_ptr<CountryModel> countryModel_;
std::shared_ptr<LanguageModel> languageModel_;
std::shared_ptr<ProblemModel> problemModel_;
std::shared_ptr<UserModel> userModel_;
};

#endif // VIEWMODELS_H
