/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "VerdictResultStore.h"

using namespace Wt;

VerdictResultStore::VerdictResultStore(DBModel *dbmodel) : dbmodel_(dbmodel) {

	VerdictResults verdictResults = dbmodel_->getVerdictResults();
	Dbo::Transaction t = dbmodel_->startTransaction();

	for(const auto &i: verdictResults) {
		verdictResultData_[i.id()].id = i.id();
		verdictResultData_[i.id()].longname = i->longname;
		verdictResultData_[i.id()].shortname = i->shortname;
	}
}

const std::string VerdictResultStore::getVerdictResultLongname(long long id) {

	if(!verdictResultData_.count(id)) return "Undefined verdict";

	return verdictResultData_.at(id).longname;
}

const std::string VerdictResultStore::getVerdictResultShortname(long long id) {

	if(!verdictResultData_.count(id)) return "UD";

	return verdictResultData_.at(id).shortname;
}
