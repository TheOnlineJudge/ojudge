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
#include "viewmodels/CountryModel.h"

class ProfileWidget : public Wt::WContainerWidget {
public:
ProfileWidget(Session *session, DBModel *dbmodel, const std::shared_ptr<CountryModel> countrymodel);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
const std::shared_ptr<CountryModel> countrymodel_;
Wt::Signal<Wt::Auth::Login&>& loginSignal() {
        return loginSignal_;
}

Wt::Signal<>& logoutSignal() {
        return logoutSignal_;
}

Wt::Signal<Wt::Auth::Login&> loginSignal_;
Wt::Signal<> logoutSignal_;

class AccountWidget : public Wt::WContainerWidget {
public:
AccountWidget(Session *session, DBModel *dbmodel, const std::shared_ptr<CountryModel> countrymodel);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
const std::shared_ptr<CountryModel> countrymodel_;
};

class SecurityWidget : public Wt::WContainerWidget {
public:
SecurityWidget(Session *session, DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
};

class NotificationsWidget : public Wt::WContainerWidget {
public:
NotificationsWidget(Session *session, DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
};

class EditorWidget : public Wt::WContainerWidget {
public:
EditorWidget(Session *session, DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
};

};
#endif // PROFILEWIDGET_H
