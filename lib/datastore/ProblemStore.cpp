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

#include "ProblemStore.h"

std::mutex addProblem_mutex;
std::mutex setCategories_mutex;

using namespace Wt;

ProblemStore::ProblemStore(DBModel *dbModel) : dbModel_(dbModel) {

	Problems problems = dbModel_->getProblems();
	Dbo::Transaction transaction = dbModel_->startTransaction();

	int row = 0;

	for(Problems::const_iterator i = problems.begin(); i != problems.end(); i++) {
		dbo::ptr<Problem> problem = *i;
		problemDataIndex_[problem->id] = row;
		problemData_[row].id = problem->id;
		problemData_[row].title = problem->title;
		for(Categories::const_iterator j = problem->categories.begin(); j != problem->categories.end(); j++) {
			dbo::ptr<Category> category = *j;
			problemData_[row].categories += std::string("#" + std::to_string(category.id()));
			problemData_[row].categoriesSet.insert(category.id());
		}
		problemData_[row].categories += std::string("#");
		row++;
	}
}

const std::map<long long,ProblemData>& ProblemStore::getProblems() {
	return problemData_;
}

const ProblemData& ProblemStore::getProblem(long long id) {
	return problemData_.at(id);
}

void ProblemStore::addProblem(long long id, std::string title, const WModelIndex& parent) {

	std::lock_guard<std::mutex> guard(addProblem_mutex);

	dbo::ptr<Problem> problem = dbModel_->addProblem(id,title);

	ProblemData tmpProblemData;
	tmpProblemData.id = id;
	tmpProblemData.title = title;

	int row = problemData_.size();
	problemData_[row] = tmpProblemData;
	problemDataIndex_[id] = row;

	auto server = Wt::WServer::instance();
	server->postAll([=] {
		auto app = dynamic_cast<ojudgeApp*>(Wt::WApplication::instance());
		assert(app != nullptr);
		app->getViewModels()->getProblemModel()->insertProblem(row,parent);
		app->triggerUpdate();
	});
}

void ProblemStore::setCategories(long long id, const std::set<int>& categories) {

	std::lock_guard<std::mutex> guard(setCategories_mutex);

	dbModel_->setProblemCategories(id,categories);

	std::string tmpStr;
	for(auto tmpCat: categories) {
		tmpStr += std::string("#" + std::to_string(tmpCat));
	}
	tmpStr += std::string("#");

	problemData_[problemDataIndex_.at(id)].categories = tmpStr;
}

const std::set<int> ProblemStore::getCategories(long long id) {

	return problemData_[problemDataIndex_.at(id)].categoriesSet;
}
