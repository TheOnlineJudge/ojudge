/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <Wt/WContainerWidget.h>
#include "dbmodel/DBModel.h"

class ProfileWidget : public Wt::WContainerWidget {
public:
ProfileWidget(Session *session);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;

};

class AccountWidget : public Wt::WContainerWidget {
public:
AccountWidget(Session *session);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
};

class SecurityWidget : public Wt::WContainerWidget {
public:
SecurityWidget(Session *session);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
};

class NotificationsWidget : public Wt::WContainerWidget {
public:
NotificationsWidget(Session *session);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
};

#endif // PROFILEWIDGET_H
