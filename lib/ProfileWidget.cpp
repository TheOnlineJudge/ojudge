/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WFont.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WMessageBox.h>
#include <Wt/WTable.h>
#include <Wt/WRadioButton.h>
#include <Wt/WFileUpload.h>
#include <Wt/Utils.h>
#include "viewmodels/CountryModel.h"
#include "ProfileWidget.h"

using namespace Wt;

ProfileWidget::ProfileWidget(Session *session, DBModel *dbmodel, const std::shared_ptr<CountryModel> countrymodel, AuthWidget* authWidget) : session_(session), dbmodel_(dbmodel), countrymodel_(countrymodel) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setPreferredImplementation(LayoutImplementation::JavaScript);

	auto pageTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Profile"),0);
	pageTitle->addStyleClass("oj-pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);
	auto menuLayout = mainWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
	menuLayout->setContentsMargins(0,0,0,0);
	menuLayout->setPreferredImplementation(LayoutImplementation::JavaScript);

	auto mainStack = menuLayout->addWidget(cpp14::make_unique<WStackedWidget>(),1);
	auto menuWidget = menuLayout->insertWidget(0,cpp14::make_unique<WMenu>(mainStack),0);
	menuWidget->addStyleClass("flex-column");
	menuWidget->setWidth(200);

	auto accountWidget = cpp14::make_unique<AccountWidget>(session_,dbmodel_,countrymodel_);
	loginSignal().connect(accountWidget.get(),&AccountWidget::login);
	logoutSignal().connect(accountWidget.get(),&AccountWidget::logout);
	auto accountItem = menuWidget->addItem("Account",std::move(accountWidget));

	auto securityWidget = cpp14::make_unique<SecurityWidget>(session_,dbmodel_,authWidget);
	loginSignal().connect(securityWidget.get(),&SecurityWidget::login);
	logoutSignal().connect(securityWidget.get(),&SecurityWidget::logout);
	auto securityItem = menuWidget->addItem("Security",std::move(securityWidget));

	auto notificationsWidget = cpp14::make_unique<NotificationsWidget>(session_,dbmodel_);
	loginSignal().connect(notificationsWidget.get(),&NotificationsWidget::login);
	logoutSignal().connect(notificationsWidget.get(),&NotificationsWidget::logout);
	auto notificationsItem = menuWidget->addItem("Notifications",std::move(notificationsWidget));
}

void ProfileWidget::login(Auth::Login& login) {
	loginSignal().emit(login);
}

void ProfileWidget::logout() {
	logoutSignal().emit();
}

ProfileWidget::AccountWidget::AccountWidget(Session *session, DBModel *dbmodel, const std::shared_ptr<CountryModel> countrymodel) : session_(session), dbmodel_(dbmodel), countrymodel_(countrymodel) {

	setTemplateText(WString::tr("settings-account"));

	addFunction("block",&WTemplate::Functions::block);
	addFunction("tr",&WTemplate::Functions::tr);
	addFunction("id",&WTemplate::Functions::id);

	auto avatarImage = cpp14::make_unique<WImage>();
	avatarImage->resize(128,128);
	avatarImage_ = bindWidget("avatar-image",std::move(avatarImage));

	auto avatarUpload = cpp14::make_unique<WContainerWidget>();
	avatarUpload->addStyleClass("oj-settings-avatar-upload");
	avatarUpload->setMargin(0);
	auto avatarLayout = avatarUpload->setLayout(cpp14::make_unique<WVBoxLayout>());
	avatarLayout->setContentsMargins(0,0,0,0);
	auto avatarFileLayout = avatarLayout->addLayout(cpp14::make_unique<WHBoxLayout>());
	avatarFileLayout->setContentsMargins(0,0,0,0);
	avatarFileLayout->addWidget(cpp14::make_unique<WText>("<label>" + tr("upload-avatar-label") + "</label>"));
	auto avatarFileUpload = avatarFileLayout->addWidget(cpp14::make_unique<WFileUpload>(),1);
	auto avatarMessage = avatarLayout->addWidget(cpp14::make_unique<WText>("<label></label><i>" + tr("upload-avatar-message") + "</i>"));
	avatarMessage->setMargin(0);
	avatarUpload_ = bindWidget("upload-avatar",std::move(avatarUpload));

	auto avatarGroup = std::make_shared<WButtonGroup>();
	auto avatarDefaultButton = cpp14::make_unique<WRadioButton>("Default");
	avatarDefaultButton->addStyleClass("oj-avatar-radiobutton");
	auto avatarGravatarButton = cpp14::make_unique<WRadioButton>("Gravatar / Libravatar");
	avatarGravatarButton->addStyleClass("oj-avatar-radiobutton");
	auto avatarCustomButton = cpp14::make_unique<WRadioButton>("Custom");
	avatarCustomButton->addStyleClass("oj-avatar-radiobutton");
	avatarGroup->addButton(bindWidget("avatar-none-setting",std::move(avatarDefaultButton)),(int)AvatarType::Default);
	avatarGroup->addButton(bindWidget("avatar-gravatar-setting",std::move(avatarGravatarButton)),(int)AvatarType::Gravatar);
	avatarGroup->addButton(bindWidget("avatar-custom-setting",std::move(avatarCustomButton)),(int)AvatarType::Custom);
	avatarGroup->checkedChanged().connect( [=] {
		avatarChanged_ = true;
		if((AvatarType)avatarGroup->checkedId() == AvatarType::Custom) {
			avatarUpload_->removeStyleClass("hidden");
		} else {
			avatarUpload_->addStyleClass("hidden");
		}
		Dbo::Transaction transaction = dbmodel_->startTransaction();
		dbo::ptr<User> userData = session_->user(login_->user());
		avatarImage_->setImageLink(userData->avatarLink((AvatarType)avatarGroup->checkedId()));
	});
	avatarGroup_ = avatarGroup.get();

	auto username = cpp14::make_unique<WLineEdit>();
	username->disable();
	username_ = bindWidget("username-setting",std::move(username));

	auto email = cpp14::make_unique<WLineEdit>();
	email->changed().connect( [=] {
		emailChanged_ = true;
	});
	email_ = bindWidget("email-setting",std::move(email));

	auto firstname = cpp14::make_unique<WLineEdit>();
	firstname->changed().connect( [=] {
		firstnameChanged_ = true;
	});
	firstname_ = bindWidget("firstname-setting",std::move(firstname));

	auto lastname = cpp14::make_unique<WLineEdit>();
	lastname->changed().connect( [=] {
		lastnameChanged_ = true;
	});
	lastname_ = bindWidget("lastname-setting",std::move(lastname));

	auto birthday = cpp14::make_unique<WDatePicker>();
	birthday->lineEdit()->disable();
	birthday->changed().connect( [=] {
		birthdayChanged_ = true;
	});
	birthday_ = bindWidget("birthday-setting",std::move(birthday));

	auto country = cpp14::make_unique<WComboBox>();
	country->setModel(countrymodel_);
	country_ = bindWidget("country-setting",std::move(country));
	auto countryFlag = cpp14::make_unique<WImage>();
	countryFlag->addStyleClass("oj-country-flag");
	countryFlag_ = bindWidget("country-flag",std::move(countryFlag));
	country_->changed().connect( [=] {
		countryFlag_->setImageLink(cpp17::any_cast<std::string>(countrymodel_->data(countrymodel_->index(country_->currentIndex(),0),ItemDataRole::Decoration)));
	});
	country_->changed().connect( [=] {
		countryChanged_ = true;
	});

	auto institution = cpp14::make_unique<WLineEdit>();
	institution->changed().connect( [=] {
		institutionChanged_ = true;
	});
	institution_ = bindWidget("institution-setting",std::move(institution));

	auto uvaid = cpp14::make_unique<WLineEdit>();
	uvaid->disable();
	uvaid_ = bindWidget("uvaid-setting",std::move(uvaid));

	auto applyButton = cpp14::make_unique<WPushButton>("Apply");
	applyButton->addStyleClass("btn-primary");
	applyButton->clicked().connect(this,&ProfileWidget::AccountWidget::applyClicked);
	bindWidget("apply-button",std::move(applyButton));

	auto resetButton = cpp14::make_unique<WPushButton>("Reset");
	resetButton->clicked().connect(this,&ProfileWidget::AccountWidget::resetClicked);
	bindWidget("reset-button",std::move(resetButton));
}

void ProfileWidget::AccountWidget::login(Auth::Login& login) {

	login_ = &login;
	reset();
}

void ProfileWidget::AccountWidget::logout() {

}

void ProfileWidget::AccountWidget::reset() {

	Dbo::Transaction transaction = dbmodel_->startTransaction();
	dbo::ptr<User> userData = session_->user(login_->user());

	avatarGroup_->button((int)userData->avatar->avatarType)->setChecked();
	avatarImage_->setImageLink(userData->avatarLink(userData->avatar->avatarType));
	if((AvatarType)userData->avatar->avatarType == AvatarType::Custom) {
		avatarUpload_->removeStyleClass("hidden");
	} else {
		avatarUpload_->addStyleClass("hidden");
	}
	avatarChanged_ = false;
	username_->setText(login_->user().identity("loginname"));
	email_->setText(login_->user().email());
	emailChanged_ = false;
	firstname_->setText(userData->firstName.value_or(""));
	firstnameChanged_ = false;
	lastname_->setText(userData->lastName.value_or(""));
	lastnameChanged_ = false;
	birthday_->setDate(userData->birthday.value_or(WDate()));
	birthdayChanged_ = false;
	country_->setCurrentIndex(countrymodel_->match(countrymodel_->index(0,0),CountryModel::CountryCodeRole,userData->country.value_or("00"),1,MatchFlag::Exactly|MatchFlag::Wrap).at(0).row());
	countryFlag_->setImageLink(cpp17::any_cast<std::string>(countrymodel_->data(countrymodel_->index(country_->currentIndex(),0),ItemDataRole::Decoration)));
	countryChanged_ = false;
	institution_->setText(userData->institution.value_or(""));
	institutionChanged_ = false;
	uvaid_->setText(userData->uvaID.value_or("none"));
}

void ProfileWidget::AccountWidget::resetClicked() {

	auto warningBox = addChild(cpp14::make_unique<WMessageBox>("Are you sure?","All changes will be lost. Do you want to continue?",
	                                                           Icon::Warning,StandardButton::Yes | StandardButton::No));
	warningBox->buttonClicked().connect( [=] (StandardButton button) {
		switch(button) {
		case StandardButton::Yes:
			reset();
			break;
		case StandardButton::No:
			break;
		}
		removeChild(warningBox);
	});
	warningBox->show();
}

void ProfileWidget::AccountWidget::applyClicked() {

	if(!avatarChanged_ && !emailChanged_ && !firstnameChanged_ && !lastnameChanged_ && !birthdayChanged_ && !countryChanged_ && !institutionChanged_) return;

	WStringStream strm;

	strm << "The following data will be updated:<br/><br/>";
	strm << "<ul>";
	if(avatarChanged_) strm << "<li>Avatar changed to: <b>" << avatarGroup_->checkedButton()->text().toUTF8() << "</b></li>";
	if(emailChanged_) strm << "<li>eMail address to: <b>" << email_->text().toUTF8() << "</b></li>";
	if(firstnameChanged_) strm << "<li>First Name to: <b>" << firstname_->text().toUTF8() << "</b></li>";
	if(lastnameChanged_) strm << "<li>Last Name to: <b>" << lastname_->text().toUTF8() << "</b></li>";
	if(birthdayChanged_) strm << "<li>Birthday to: <b>" << birthday_->lineEdit()->text().toUTF8() << "</b></li>";
	if(countryChanged_) strm << "<li>Country to: <b>" << cpp17::any_cast<std::string>(countrymodel_->data(countrymodel_->index(country_->currentIndex(),0))) << "</b></li>";
	if(institutionChanged_) strm << "<li>Institution/Company to: <b>" << institution_->text().toUTF8() << "</b></li>";
	strm << "</ul>";
	strm << "<br/>Do you want to continue?";

	auto warningBox = addChild(cpp14::make_unique<WMessageBox>("Are you sure?","",Icon::Information,StandardButton::Yes | StandardButton::No));
	warningBox->textWidget()->setTextFormat(TextFormat::XHTML);
	warningBox->setText(strm.str());

	warningBox->buttonClicked().connect( [=] (StandardButton button) {
		switch(button) {
		case StandardButton::Yes:
			{
			        Dbo::Transaction transaction = dbmodel_->startTransaction();
			        dbo::ptr<User> userData = session_->user(login_->user());
			        if(avatarChanged_) userData->avatar.modify()->avatarType = (AvatarType)avatarGroup_->checkedId();
			        if(emailChanged_) myAuthService.verifyEmailAddress(login_->user(),email_->text().toUTF8());
			        if(firstnameChanged_) userData.modify()->firstName = firstname_->text().toUTF8();
			        if(lastnameChanged_) userData.modify()->lastName = lastname_->text().toUTF8();
			        if(birthdayChanged_) userData.modify()->birthday = birthday_->date();
			        if(countryChanged_) userData.modify()->country = cpp17::any_cast<std::string>(countrymodel_->data(countrymodel_->index(country_->currentIndex(),0),
					                                                                                          CountryModel::CountryCodeRole));
			        if(institutionChanged_) userData.modify()->institution = institution_->text().toUTF8();
			}
			break;
		case StandardButton::No:
			break;
		}
		removeChild(warningBox);
	});
	warningBox->show();
}

ProfileWidget::SecurityWidget::SecurityWidget(Session *session, DBModel *dbmodel, AuthWidget *authWidget) : session_(session), dbmodel_(dbmodel), authWidget_(authWidget) {

	setTemplateText(WString::tr("settings-security"));

	addFunction("block",&WTemplate::Functions::block);
	addFunction("tr",&WTemplate::Functions::tr);
	addFunction("id",&WTemplate::Functions::id);

	auto changePassword = cpp14::make_unique<WPushButton>();
	changePassword->setText("Click here");
	changePassword->clicked().connect( [=] {
		authWidget_->letUpdatePassword(login_->user(),true);
	});
	changePassword_ = bindWidget("changepassword-setting",std::move(changePassword));

	auto twofa = cpp14::make_unique<WPushButton>();
	twofa->setText("Enable 2FA");
	twofa->addStyleClass("btn-primary");
	twofa_ = bindWidget("twofa-setting",std::move(twofa));
}

void ProfileWidget::SecurityWidget::login(Auth::Login& login) {

	login_ = &login;
}

void ProfileWidget::SecurityWidget::logout() {

}

ProfileWidget::NotificationsWidget::NotificationsWidget(Session *session, DBModel *dbmodel) : session_(session), dbmodel_(dbmodel) {

	setTemplateText(WString::tr("settings-notifications"));

	addFunction("block",&WTemplate::Functions::block);
	addFunction("tr",&WTemplate::Functions::tr);
	addFunction("id",&WTemplate::Functions::id);

	auto notificationsTable = cpp14::make_unique<WTable>();
	notificationsTable->addStyleClass("oj-settings-notifications");

	notificationsTable->elementAt(0,1)->addWidget(cpp14::make_unique<WText>("eMail"));
	notificationsTable->elementAt(0,2)->addWidget(cpp14::make_unique<WText>("Browser"));

	notificationsTable->elementAt(1,0)->addWidget(cpp14::make_unique<WText>("Submission results"));
	notificationsTable->elementAt(2,0)->addWidget(cpp14::make_unique<WText>("Contest announcements"));
	notificationsTable->elementAt(3,0)->addWidget(cpp14::make_unique<WText>("General information"));

	notificationsTable->setHeaderCount(1,Orientation::Horizontal);
	notificationsTable->setHeaderCount(1,Orientation::Vertical);

	emailResults_ = notificationsTable->elementAt(1,1)->addWidget(cpp14::make_unique<WCheckBox>());
	emailResults_->changed().connect( [=] {
		emailResultsChanged_ = true;
	});
	emailContests_ = notificationsTable->elementAt(2,1)->addWidget(cpp14::make_unique<WCheckBox>());
	emailContests_->changed().connect( [=] {
		emailContestsChanged_ = true;
	});
	emailGeneral_ = notificationsTable->elementAt(3,1)->addWidget(cpp14::make_unique<WCheckBox>());
	emailGeneral_->changed().connect( [=] {
		emailGeneralChanged_ = true;
	});

	browserResults_ = notificationsTable->elementAt(1,2)->addWidget(cpp14::make_unique<WCheckBox>());
	browserResults_->changed().connect( [=] {
		browserResultsChanged_ = true;
	});
	browserContests_ = notificationsTable->elementAt(2,2)->addWidget(cpp14::make_unique<WCheckBox>());
	browserContests_->changed().connect( [=] {
		browserContestsChanged_ = true;
	});
	browserGeneral_ = notificationsTable->elementAt(3,2)->addWidget(cpp14::make_unique<WCheckBox>());
	browserGeneral_->changed().connect( [=] {
		browserGeneralChanged_ = true;
	});

	bindWidget("notifications-table",std::move(notificationsTable));

	auto applyButton = cpp14::make_unique<WPushButton>("Apply");
	applyButton->addStyleClass("btn-primary");
	applyButton->clicked().connect(this,&ProfileWidget::NotificationsWidget::applyClicked);
	bindWidget("apply-button",std::move(applyButton));

	auto resetButton = cpp14::make_unique<WPushButton>("Reset");
	resetButton->clicked().connect(this,&ProfileWidget::NotificationsWidget::resetClicked);
	bindWidget("reset-button",std::move(resetButton));
}

void ProfileWidget::NotificationsWidget::login(Auth::Login& login) {

	login_ = &login;
	reset();
}

void ProfileWidget::NotificationsWidget::logout() {

}

void ProfileWidget::NotificationsWidget::reset() {

	Dbo::Transaction transaction = dbmodel_->startTransaction();
	dbo::ptr<User> userData = session_->user(login_->user());

	emailResults_->setCheckState(userData->settings->notifications_email_results.value_or(true) ? CheckState::Checked : CheckState::Unchecked);
	emailContests_->setCheckState(userData->settings->notifications_email_contests.value_or(true) ? CheckState::Checked : CheckState::Unchecked);
	emailGeneral_->setCheckState(userData->settings->notifications_email_general.value_or(true) ? CheckState::Checked : CheckState::Unchecked);

	browserResults_->setCheckState(userData->settings->notifications_browser_results.value_or(false) ? CheckState::Checked : CheckState::Unchecked);
	browserContests_->setCheckState(userData->settings->notifications_browser_results.value_or(false) ? CheckState::Checked : CheckState::Unchecked);
	browserGeneral_->setCheckState(userData->settings->notifications_browser_results.value_or(false) ? CheckState::Checked : CheckState::Unchecked);

	emailResultsChanged_ = false;
	emailContestsChanged_ = false;
	emailGeneralChanged_ = false;
	browserResultsChanged_ = false;
	browserContestsChanged_ = false;
	browserGeneralChanged_ = false;
}

void ProfileWidget::NotificationsWidget::resetClicked() {

	auto warningBox = addChild(cpp14::make_unique<WMessageBox>("Are you sure?","All changes will be lost. Do you want to continue?",
	                                                           Icon::Warning,StandardButton::Yes | StandardButton::No));
	warningBox->buttonClicked().connect( [=] (StandardButton button) {
		switch(button) {
		case StandardButton::Yes:
			reset();
			break;
		case StandardButton::No:
			break;
		}
		removeChild(warningBox);
	});
	warningBox->show();
}

void ProfileWidget::NotificationsWidget::applyClicked() {

	if(!emailResultsChanged_ && !emailContestsChanged_ && !emailGeneralChanged_ &&
	   !browserResultsChanged_ && !browserContestsChanged_ && !browserGeneralChanged_) return;

	WStringStream strm;

	strm << "The following data will be updated:<br/><br/>";
	strm << "<ul>";
	if(emailResultsChanged_) strm << "<li>eMail submission results to: <b>" << (emailResults_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(emailContestsChanged_) strm << "<li>eMail contests announcements to: <b>" << (emailContests_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(emailGeneralChanged_) strm << "<li>eMail general information to: <b>" << (emailGeneral_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(browserResultsChanged_) strm << "<li>Browser submission results to: <b>" << (browserResults_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(browserContestsChanged_) strm << "<li>Browser contests announcements to: <b>" << (browserContests_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(browserGeneralChanged_) strm << "<li>Browser general information to: <b>" << (browserGeneral_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	strm << "</ul>";
	strm << "<br/>Do you want to continue?";

	auto warningBox = addChild(cpp14::make_unique<WMessageBox>("Are you sure?","",Icon::Information,StandardButton::Yes | StandardButton::No));
	warningBox->textWidget()->setTextFormat(TextFormat::XHTML);
	warningBox->setText(strm.str());

	warningBox->buttonClicked().connect( [=] (StandardButton button) {
		switch(button) {
		case StandardButton::Yes:
			{
			        Dbo::Transaction transaction = dbmodel_->startTransaction();
			        dbo::ptr<User> userData = session_->user(login_->user());
			        if(emailResultsChanged_) userData->settings.modify()->notifications_email_results = (emailResults_->isChecked() ? true : false);
			        if(emailContestsChanged_) userData->settings.modify()->notifications_email_contests = (emailContests_->isChecked() ? true : false);
			        if(emailGeneralChanged_) userData->settings.modify()->notifications_email_general = (emailGeneral_->isChecked() ? true : false);
			        if(browserResultsChanged_) userData->settings.modify()->notifications_browser_results = (browserResults_->isChecked() ? true : false);
			        if(browserContestsChanged_) userData->settings.modify()->notifications_browser_contests = (browserContests_->isChecked() ? true : false);
			        if(browserGeneralChanged_) userData->settings.modify()->notifications_browser_general = (browserGeneral_->isChecked() ? true : false);
			}
			break;
		case StandardButton::No:
			break;
		}
		removeChild(warningBox);
	});
	warningBox->show();
}
