/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#ifndef PROBLEMSWIDGET_H
#define PROBLEMSWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WSortFilterProxyModel.h>

class ViewModels;

class ProblemsWidget : public Wt::WContainerWidget {
public:
	ProblemsWidget(ViewModels *viewModels);

private:
	std::shared_ptr<Wt::WSortFilterProxyModel> proxyModel_;
	ViewModels *viewModels_;

};

#endif // PROBLEMSWIDGET_H
