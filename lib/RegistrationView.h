/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef REGISTRATIONVIEW_H
#define REGISTRATIONVIEW_H

#include <Wt/Auth/RegistrationWidget.h>

using namespace Wt;

class Session;

class RegistrationView : public Auth::RegistrationWidget {
public:
RegistrationView(Session &session, Auth::AuthWidget *authWidget = 0);
virtual std::unique_ptr<WWidget> createFormWidget(WFormModel::Field field);

protected:
virtual bool validate();

private:
Session &session_;
};

#endif // REGISTRATIONVIEW_H
