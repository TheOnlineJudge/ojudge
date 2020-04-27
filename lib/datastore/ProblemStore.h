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
ProblemStore(DBModel *dbmodel);
const std::map<long long,ProblemData>& getProblems();
const std::optional<ProblemData> getProblemByRow(long long row);
const std::optional<ProblemData> getProblemById(long long id);
void addProblem(long long id, std::string title, const Wt::WModelIndex& parent);
void setCategories(long long id, const std::set<int>& categories);
const std::set<int> getCategories(long long id);
void updateHtmlDescription(long long id, std::string content);
void updatePdfDescription(long long id, std::vector<unsigned char> content);
std::string getHtmlDescription(long long id);
std::vector<unsigned char> getPdfDescription(long long id);

private:
DBModel *dbmodel_;
std::map<long long,ProblemData> problemData_;
std::map<long long,long long> problemDataIndex_;
};

#endif // PROBLEMSTORE_H
