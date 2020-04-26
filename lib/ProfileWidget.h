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
#include <Wt/WDialog.h>
#include <Wt/WFileUpload.h>
#include "AuthWidget.h"
#include "dbmodel/DBModel.h"
#include "datastore/DataStore.h"
#include "viewmodels/CountryModel.h"

class ProfileWidget : public Wt::WContainerWidget {
public:
ProfileWidget(Session *session, DBModel *dbmodel, DataStore *dataStore, const std::shared_ptr<CountryModel> countrymodel, AuthWidget *authWidget);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
DataStore *dataStore_;
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
void avatarUploaded();
Wt::WImage *avatarImage_;
Wt::WButtonGroup *avatarGroup_;
Wt::WContainerWidget *avatarUpload_;
Wt::WFileUpload *avatarFileUpload_;
Wt::WLineEdit *username_;
Wt::WLineEdit *email_;
Wt::WLineEdit *telegramUsername_;
Wt::WLineEdit *firstname_;
Wt::WLineEdit *lastname_;
Wt::WDatePicker *birthday_;
Wt::WComboBox *country_;
Wt::WImage *countryFlag_;
Wt::WLineEdit *institution_;
Wt::WLineEdit *uvaid_;
std::vector<unsigned char> customAvatar_;
bool avatarChanged_ = false;
bool customAvatarChanged_ = false;
bool emailChanged_ = false;
bool telegramUsernameChanged_ = false;
bool firstnameChanged_ = false;
bool lastnameChanged_ = false;
bool birthdayChanged_ = false;
bool countryChanged_ = false;
bool institutionChanged_ = false;
};

class SecurityWidget : public Wt::WTemplate {
public:
SecurityWidget(Session *session, DBModel *dbmodel, DataStore *dataStore, AuthWidget *authWidget);
void login(Wt::Auth::Login& login);
void logout();

private:
Session *session_;
DBModel *dbmodel_;
DataStore *dataStore_;
AuthWidget *authWidget_;
Wt::Auth::Login *login_;
Wt::WPushButton *changePassword_;
Wt::WPushButton *twofa_;
void twofaButtonReset();
void TwoFADialogDone(Wt::DialogCode code);
bool disableTwoFA_ = false;

class TwoFADialog : public Wt::WDialog {
public:
TwoFADialog(Session *session, SettingStore *settingStore, Wt::Auth::Login *login, bool disable);
std::vector<unsigned char> getSecret();
std::string getAuthCode();
void setCodeError();

private:
Session *session_;
SettingStore *settingStore_;
Wt::Auth::Login *login_;
Wt::WLineEdit *authCode_;
std::vector<unsigned char> secret_;
};

TwoFADialog *twofaDialog_;

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
Wt::WCheckBox *emailMessages_;
Wt::WCheckBox *browserResults_;
Wt::WCheckBox *browserContests_;
Wt::WCheckBox *browserGeneral_;
Wt::WCheckBox *browserMessages_;
Wt::WCheckBox *telegramResults_;
Wt::WCheckBox *telegramContests_;
Wt::WCheckBox *telegramGeneral_;
Wt::WCheckBox *telegramMessages_;
bool emailResultsChanged_ = false;
bool emailContestsChanged_ = false;
bool emailGeneralChanged_ = false;
bool emailMessagesChanged_ = false;
bool browserResultsChanged_ = false;
bool browserContestsChanged_ = false;
bool browserGeneralChanged_ = false;
bool browserMessagesChanged_ = false;
bool telegramResultsChanged_ = false;
bool telegramContestsChanged_ = false;
bool telegramGeneralChanged_ = false;
bool telegramMessagesChanged_ = false;
};

};
#endif // PROFILEWIDGET_H
