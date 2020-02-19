/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef DATASTORE_H
#define DATASTORE_H

class DBModel;

class CategoryStore;
class ProblemStore;

class DataStore {
public:
DataStore(DBModel *dbModel);
CategoryStore *getCategoryStore();
ProblemStore *getProblemStore();

private:
DBModel *dbModel_;
CategoryStore *categoryStore_;
ProblemStore *problemStore_;
};

#endif // DATASTORE_H
