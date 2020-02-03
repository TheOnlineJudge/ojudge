/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#include "ViewModels.h"

ViewModels::ViewModels(DBModel *dbmodel) : dbmodel_(dbmodel) {

	categoryModel_ = std::make_shared<CategoryModel>(dbmodel_) ;

}

const std::shared_ptr<CategoryModel> ViewModels::getCategoryModel() {
	return categoryModel_ ;
}
