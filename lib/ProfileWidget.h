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
#include <Wt/WVBoxLayout.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WDatePicker.h>
#include <Wt/WComboBox.h>
#include <Wt/WImage.h>
#include <Wt/WCheckBox.h>
#include <Wt/WButtonGroup.h>
#include "AuthWidget.h"
#include "dbmodel/DBModel.h"
#include "viewmodels/CountryModel.h"

class ProfileWidget : public Wt::WContainerWidget {
public:
ProfileWidget(Session *session, DBModel *dbmodel, const std::shared_ptr<CountryModel> countrymodel, AuthWidget *authWidget);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
AuthWidget *authWidget_;
Wt::WVBoxLayout *mainLayout_;
Wt::WContainerWidget *mainWidget_;
Wt::WText *mustLoginWidget_;
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
Wt::WImage *avatarImage_;
Wt::WButtonGroup *avatarGroup_;
Wt::WContainerWidget *avatarUpload_;
Wt::WLineEdit *username_;
Wt::WLineEdit *email_;
Wt::WLineEdit *firstname_;
Wt::WLineEdit *lastname_;
Wt::WDatePicker *birthday_;
Wt::WComboBox *country_;
Wt::WImage *countryFlag_;
Wt::WLineEdit *institution_;
Wt::WLineEdit *uvaid_;
bool avatarChanged_ = false;
bool emailChanged_ = false;
bool firstnameChanged_ = false;
bool lastnameChanged_ = false;
bool birthdayChanged_ = false;
bool countryChanged_ = false;
bool institutionChanged_ = false;
};

class SecurityWidget : public Wt::WTemplate {
public:
SecurityWidget(Session *session, DBModel *dbmodel, AuthWidget *authWidget);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
AuthWidget *authWidget_;
Wt::Auth::Login *login_;
Wt::WPushButton *changePassword_;
Wt::WPushButton *twofa_;
};

class NotificationsWidget : public Wt::WTemplate {
public:
NotificationsWidget(Session *session, DBModel *dbmodel);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
Wt::Auth::Login *login_;
void reset();
void resetClicked();
void applyClicked();
Wt::WCheckBox *emailResults_;
Wt::WCheckBox *emailContests_;
Wt::WCheckBox *emailGeneral_;
Wt::WCheckBox *browserResults_;
Wt::WCheckBox *browserContests_;
Wt::WCheckBox *browserGeneral_;
bool emailResultsChanged_ = false;
bool emailContestsChanged_ = false;
bool emailGeneralChanged_ = false;
bool browserResultsChanged_ = false;
bool browserContestsChanged_ = false;
bool browserGeneralChanged_ = false;
};

};
#endif // PROFILEWIDGET_H
