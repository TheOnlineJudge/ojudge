/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "../dbmodel/DBModel.h"

#include "ProblemStore.h"

using namespace Wt;

ProblemStore::ProblemStore(DBModel *dbModel) : dbModel_(dbModel) {

        Problems problems = dbModel_->getProblems();
        Dbo::Transaction transaction = dbModel_->startTransaction();

        int row = 0;

        for(Problems::const_iterator i = problems.begin(); i != problems.end(); i++) {
                dbo::ptr<Problem> problem = *i;
                problemData_[row].id = problem.id();
                problemData_[row].title = problem->title;
                for(Categories::const_iterator j = problem->categories.begin(); j != problem->categories.end(); j++) {
                        dbo::ptr<Category> category = *j;
                        problemData_[row].categories += std::string("#" + std::to_string(category.id()));
                }
                problemData_[row].categories += std::string("#");
                row++;
        }
}

const std::map<int,ProblemData>& ProblemStore::getProblems() {
        return problemData_;
}

const ProblemData& ProblemStore::getProblem(int id) {
	return problemData_.at(id);
}
