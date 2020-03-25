/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef PROBLEMSTORE_H
#define PROBLEMSTORE_H

#include <string>
#include <map>
#include <Wt/WModelIndex.h>

class DBModel;

struct ProblemData {
	long long id;
	std::string title;
	std::string categories;
	std::set<int> categoriesSet;
};


class ProblemStore {
public:
ProblemStore(DBModel *dbModel);
const std::map<long long,ProblemData>& getProblems();
const ProblemData& getProblem(long long id);
void addProblem(long long id, std::string title, const Wt::WModelIndex& parent);
void setCategories(long long id, const std::set<int>& categories);
const std::set<int> getCategories(long long id);

private:
DBModel *dbModel_;
std::map<long long,ProblemData> problemData_;
std::map<long long,long long> problemDataIndex_;
};

#endif // PROBLEMSTORE_H
