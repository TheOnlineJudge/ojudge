/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#ifndef ADMINPROBLEMWIDGET_H
#define ADMINPROBLEMWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WTableView.h>
#include <Wt/WSortFilterProxyModel.h>
#include "viewmodels/ProblemModel.h"

class AdminProblemWidget : public Wt::WContainerWidget {
public:
	AdminProblemWidget(const std::shared_ptr<ProblemModel> probmodel) ;

private:
	Wt::WVBoxLayout *mainLayout_;
	Wt::WTableView *tableWidget_;
	std::shared_ptr<Wt::WSortFilterProxyModel> proxyModel_;
};

#endif // ADMINPROBLEMWIDGET_H
