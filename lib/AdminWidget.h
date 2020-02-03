/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WAbstractItemModel.h>

class Session;
class ViewModels;

class AdminWidget : public Wt::WContainerWidget {
public:
	AdminWidget(Session *session, ViewModels *viewModels);

private:
	Session *session_;
	ViewModels *viewModels_;

};

#endif // ADMINWIDGET_H
