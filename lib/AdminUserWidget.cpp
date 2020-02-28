/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WHBoxLayout.h>
#include <Wt/WImage.h>
#include <Wt/WCssDecorationStyle.h>
#include "AdminUserWidget.h"

using namespace Wt;

AdminUserWidget::AdminUserWidget() {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout_->setContentsMargins(0,0,0,0);
}
