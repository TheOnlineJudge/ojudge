/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef ADMINSETTINGSWIDGET_H
#define ADMINSETTINGSWIDGET_H

#include <Wt/WContainerWidget.h>

class DBModel;

class AdminSettingsWidget : public Wt::WContainerWidget {
public:
AdminSettingsWidget(DBModel *dbmodel);

private:
DBModel *dbmodel_;

};

#endif // ADMINSETTINGSWIDGET_H
