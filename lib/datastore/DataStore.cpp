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
#include "ProblemStore.h"

DataStore::DataStore(DBModel *dbModel) : dbModel_(dbModel) {

	categoryStore_ = new CategoryStore(dbModel_);
	problemStore_ = new ProblemStore(dbModel_);

}

CategoryStore *DataStore::getCategoryStore() {
	return categoryStore_;
}

ProblemStore *DataStore::getProblemStore() {
	return problemStore_;
}
