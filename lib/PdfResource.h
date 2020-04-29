/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef PDFRESOURCE_H
#define PDFRESOURCE_H

#include <Wt/WMemoryResource.h>
#include "datastore/DataStore.h"

class PdfResource : public Wt::WMemoryResource {
public:
PdfResource(ProblemStore *problemStore);
void setProblem(long long id);

private:
long long id_;
ProblemStore *problemStore_;
virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override;
};

#endif // PDFRESOURCE_H
