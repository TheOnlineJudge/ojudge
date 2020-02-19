/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WServer.h>
#include <Wt/WApplication.h>
#include <mutex>
#include "../ojudgeApp.h"

#include "../dbmodel/DBModel.h"

#include "CategoryStore.h"

std::mutex addCategory_mutex;

using namespace Wt;

CategoryStore::CategoryStore(DBModel *dbModel) : dbModel_(dbModel) {

	std::map<int,int> rowCount;
	Categories categories = dbModel_->getCategories();
	Dbo::Transaction transaction = dbModel_->startTransaction();

	typedef std::vector< Dbo::ptr<Category> > tmpCategories;
	tmpCategories tmpcategories(categories.begin(), categories.end());

        for(tmpCategories::const_iterator i = tmpcategories.begin(); i != tmpcategories.end(); i++) {
                dbo::ptr<Category> category = *i;
                rowCount[category.id()] = 0;
        }

        for(tmpCategories::const_iterator i = tmpcategories.begin(); i != tmpcategories.end(); i++) {
                dbo::ptr<Category> category = *i;
                rowCount[category->parent.id()]++;
        }

        categoryData_[1].title = std::string("Root category");
        categoryData_[1].parent = -1;
        categoryData_[1].categories = rowCount.at(1);
        categoryData_[1].problems = 0;

        for(tmpCategories::const_iterator i = tmpcategories.begin(); i != tmpcategories.end(); i++) {
                dbo::ptr<Category> category = *i;
                if(category.id()>1) {
                        categoryData_[category.id()].title = category->title;
                        categoryData_[category.id()].parent = category->parent.id();
                        categoryData_[category.id()].categories = rowCount.at(category.id());
                        categoryData_[category.id()].problems = category->problems.size();
                }
        }
}

const std::map<int,CategoryData>& CategoryStore::getCategories() {
	return categoryData_;
}

const CategoryData& CategoryStore::getCategory(int id) {
	return categoryData_.at(id);
}

void CategoryStore::addCategory(std::string title, Wt::WModelIndex& parent, int row) {

	std::lock_guard<std::mutex> guard(addCategory_mutex);

	int parentId = (parent.isValid()?cpp17::any_cast<int>(parent.data(CategoryModel::CategoryIdRole)):1);
	dbo::ptr<Category> category = dbModel_->addCategory(title,parentId);

        categoryData_[category.id()].title = title;
        categoryData_[category.id()].parent = parentId;
        categoryData_[category.id()].categories = 0;
        categoryData_[category.id()].problems = 0;

	categoryData_[parentId].categories++;

	auto server = Wt::WServer::instance();
	server->postAll([=]{
		auto app = dynamic_cast<ojudgeApp*>(Wt::WApplication::instance());
		assert(app != nullptr);
		app->getViewModels()->getCategoryModel()->insertCategory(row,parent,category.id());
		app->triggerUpdate();
	});
}
