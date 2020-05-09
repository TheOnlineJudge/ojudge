/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef SUBMISSIONSTORE_H
#define SUBMISSIONSTORE_H

#include "../dbmodel/DBModel.h"

class SubmissionStore {
public:
SubmissionStore(DBModel *dbModel);

private:
DBModel *dbmodel_;
};

#endif // SUBMISSIONSTORE_H
