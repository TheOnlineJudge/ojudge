/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef VERDICTRESULTSTORE_H
#define VERDICTRESULTSTORE_H

#include "../dbmodel/DBModel.h"

struct VerdictResultData {
	long long id;
	std::string longname;
	std::string shortname;
};

class VerdictResultStore {
public:
VerdictResultStore(DBModel *dbModel);
const std::string getVerdictResultLongname(long long id);
const std::string getVerdictResultShortname(long long id);

private:
DBModel *dbmodel_;
std::map<long long,VerdictResultData> verdictResultData_;
};

#endif // VERDICTRESULTSTORE_H
