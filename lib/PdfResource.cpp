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
#include "datastore/ProblemStore.h"

using namespace Wt;

PdfResource::PdfResource(ProblemStore *problemStore) : problemStore_(problemStore) {

}

void PdfResource::setProblem(long long id) {
	id_ = id;
}

void PdfResource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {

	response.setMimeType("application/pdf");
	response.addHeader("Content-Disposition","attachment; filename=\"" + std::to_string(id_) + ".pdf\"");
	response.setContentLength(problemStore_->getPdfDescription(id_).size());

	for(const auto &i: problemStore_->getPdfDescription(id_)) {
		response.out() << i;
	}
}
