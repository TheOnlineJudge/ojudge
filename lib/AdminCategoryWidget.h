/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#ifndef ADMINCATEGORYWIDGET_H
#define ADMINCATEGORYWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WTreeView.h>
#include "viewmodels/CategoryModel.h"

class AdminCategoryWidget : public Wt::WContainerWidget {
public:
	AdminCategoryWidget(const std::shared_ptr<CategoryModel> catmodel) ;

private:
	Wt::WVBoxLayout *mainLayout_;
	Wt::WTreeView *treeWidget_;
};

#endif // ADMINCATEGORYWIDGET_H
