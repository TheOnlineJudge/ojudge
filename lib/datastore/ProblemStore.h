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

class DBModel;

struct ProblemData {
        long long id;
        std::string title;
        std::string categories;
};


class ProblemStore {
public:
ProblemStore(DBModel *dbModel);
const std::map<int,ProblemData>& getProblems();
const ProblemData& getProblem(int id);

private:
DBModel *dbModel_;
std::map<int,ProblemData> problemData_;
};

#endif // PROBLEMSTORE_H
