/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "../datastore/DataStore.h"
#include "ViewModels.h"

ViewModels::ViewModels(DataStore *dataStore) : dataStore_(dataStore) {

	categoryModel_ = std::make_shared<CategoryModel>(dataStore_->getCategoryStore());
	languageModel_ = std::make_shared<LanguageModel>(dataStore_->getLanguageStore());
	problemModel_ = std::make_shared<ProblemModel>(dataStore_->getProblemStore());

}

const std::shared_ptr<CategoryModel> ViewModels::getCategoryModel() {
	return categoryModel_;
}

const std::shared_ptr<LanguageModel> ViewModels::getLanguageModel() {
	return languageModel_;
}

const std::shared_ptr<ProblemModel> ViewModels::getProblemModel() {
	return problemModel_;
}
