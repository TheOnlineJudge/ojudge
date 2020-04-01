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
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WDatePicker.h>
#include <Wt/WComboBox.h>
#include <Wt/WImage.h>
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

class AccountWidget : public Wt::WTemplate {
public:
AccountWidget(Session *session, DBModel *dbmodel, const std::shared_ptr<CountryModel> countrymodel);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
Wt::Auth::Login *login_;
const std::shared_ptr<CountryModel> countrymodel_;
void reset();
void resetClicked();
void applyClicked();
Wt::WLineEdit *username_;
Wt::WLineEdit *email_;
Wt::WLineEdit *firstname_;
Wt::WLineEdit *lastname_;
Wt::WDatePicker *birthday_;
Wt::WComboBox *country_;
Wt::WImage *countryFlag_;
Wt::WLineEdit *institution_;
Wt::WLineEdit *uvaid_;
bool emailChanged_ = false;
bool firstnameChanged_ = false;
bool lastnameChanged_ = false;
bool birthdayChanged_ = false;
bool countryChanged_ = false;
bool institutionChanged_ = false;
};

class SecurityWidget : public Wt::WTemplate {
public:
SecurityWidget(Session *session, DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
};

class NotificationsWidget : public Wt::WTemplate {
public:
NotificationsWidget(Session *session, DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
};

class EditorWidget : public Wt::WTemplate {
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
