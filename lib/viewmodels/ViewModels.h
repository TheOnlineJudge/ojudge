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
#include "ProblemModel.h"

class DataStore;

class ViewModels {
public:
ViewModels(DataStore *dataStore);
const std::shared_ptr<CategoryModel> getCategoryModel();
const std::shared_ptr<ProblemModel> getProblemModel();

private:
DataStore *dataStore_;
std::shared_ptr<CategoryModel> categoryModel_;
std::shared_ptr<ProblemModel> problemModel_;
};

#endif // VIEWMODELS_H
