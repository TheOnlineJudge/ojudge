/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef STATISTICSSTORE_H
#define STATISTICSSTORE_H

#include "../dbmodel/DBModel.h"

class StatisticsStore {
public:
StatisticsStore(DBModel *dbModel);

private:
DBModel *dbmodel_;
};

#endif // STATISTICSSTORE_H
