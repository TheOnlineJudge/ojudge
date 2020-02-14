/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef ADMINLANGUAGEWIDGET_H
#define ADMINLANGUAGEWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>

class AdminLanguageWidget : public Wt::WContainerWidget {
public:
AdminLanguageWidget();

private:
Wt::WVBoxLayout *mainLayout_;
};

#endif // ADMINLANGUAGEWIDGET_H
