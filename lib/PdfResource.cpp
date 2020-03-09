/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>
#include "PdfResource.h"
#include "dbmodel/DBModel.h"

using namespace Wt;

PdfResource::PdfResource(DBModel *dbmodel) : dbmodel_(dbmodel) {

}

void PdfResource::setProblemId(long long id) {
	id_ = id;
}

void PdfResource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {

	dbo::ptr<Problem> problemData = dbmodel_->getProblem(id_);

	Dbo::Transaction transaction = dbmodel_->startTransaction();
	dbo::ptr<Description> desc = problemData->description;

	response.setMimeType("application/pdf");
	response.addHeader("Content-Disposition","attachment; filename=\"" + std::to_string(id_) + ".pdf\"");
	response.setContentLength(desc->pdfdata.value().size());

	for(const auto &i: desc->pdfdata.value()) {
		response.out() << i;
	}
}
