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

using namespace Wt;

PdfResource::PdfResource(DBModel *dbmodel) : dbmodel_(dbmodel) {

}

void PdfResource::setProblem(dbo::ptr<Problem> problemData) {
	problemData_ = problemData;
}

void PdfResource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {

	Dbo::Transaction transaction = dbmodel_->startTransaction();
	dbo::ptr<Description> desc = problemData_->description;

	response.setMimeType("application/pdf");
	response.addHeader("Content-Disposition","attachment; filename=\"" + std::to_string(problemData_.id()) + ".pdf\"");
	response.setContentLength(desc->pdfdata.value().size());

	for(const auto &i: desc->pdfdata.value()) {
		response.out() << i;
	}
}
