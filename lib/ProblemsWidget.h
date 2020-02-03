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

class ViewModels;

class ProblemsWidget : public Wt::WContainerWidget {
public:
	ProblemsWidget(ViewModels *viewModels);

private:
	ViewModels *viewModels_;

};

#endif // PROBLEMSWIDGET_H
