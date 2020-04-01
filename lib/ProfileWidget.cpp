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
#include "viewmodels/CountryModel.h"
#include "ProfileWidget.h"

using namespace Wt;

ProfileWidget::ProfileWidget(Session *session, DBModel *dbmodel, const std::shared_ptr<CountryModel> countrymodel) : session_(session), dbmodel_(dbmodel), countrymodel_(countrymodel) {

	auto mainLayout = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	auto pageTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Profile"),0);
	pageTitle->addStyleClass("oj-pagetitle");

	auto mainWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);
	auto menuLayout = mainWidget->setLayout(cpp14::make_unique<WHBoxLayout>());

	auto mainStack = menuLayout->addWidget(cpp14::make_unique<WStackedWidget>(),1);
	auto menuWidget = menuLayout->insertWidget(0,cpp14::make_unique<WMenu>(mainStack),0);
	menuWidget->addStyleClass("flex-column");
	menuWidget->setWidth(200);

	auto accountWidget = cpp14::make_unique<AccountWidget>(session_,dbmodel_,countrymodel_);
	loginSignal().connect(accountWidget.get(),&AccountWidget::login);
	logoutSignal().connect(accountWidget.get(),&AccountWidget::logout);
	auto accountItem = menuWidget->addItem("Account",std::move(accountWidget));

	auto securityWidget = cpp14::make_unique<SecurityWidget>(session_,dbmodel_);
	loginSignal().connect(securityWidget.get(),&SecurityWidget::login);
	logoutSignal().connect(securityWidget.get(),&SecurityWidget::logout);
	auto securityItem = menuWidget->addItem("Security",std::move(securityWidget));

	auto notificationsWidget = cpp14::make_unique<NotificationsWidget>(session_,dbmodel_);
	loginSignal().connect(notificationsWidget.get(),&NotificationsWidget::login);
	logoutSignal().connect(notificationsWidget.get(),&NotificationsWidget::logout);
	auto notificationsItem = menuWidget->addItem("Notifications",std::move(notificationsWidget));

	auto editorWidget = cpp14::make_unique<EditorWidget>(session_,dbmodel_);
	loginSignal().connect(editorWidget.get(),&EditorWidget::login);
	logoutSignal().connect(editorWidget.get(),&EditorWidget::logout);
	auto editorItem = menuWidget->addItem("Code Editor",std::move(editorWidget));
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

	auto username = cpp14::make_unique<WLineEdit>();
	username->disable();
	username_ = username.get();
	bindWidget("username-setting",std::move(username));

	auto email = cpp14::make_unique<WLineEdit>();
	email->changed().connect( [=] { emailChanged_ = true; });
	email_ = email.get();
	bindWidget("email-setting",std::move(email));

	auto firstname = cpp14::make_unique<WLineEdit>();
	firstname->changed().connect( [=] { firstnameChanged_ = true; });
	firstname_ = firstname.get();
	bindWidget("firstname-setting",std::move(firstname));

	auto lastname = cpp14::make_unique<WLineEdit>();
	lastname->changed().connect( [=] { lastnameChanged_ = true; });
	lastname_ = lastname.get();
	bindWidget("lastname-setting",std::move(lastname));

	auto birthday = cpp14::make_unique<WDatePicker>();
	birthday->lineEdit()->disable();
	birthday->changed().connect( [=] { birthdayChanged_ = true; });
	birthday_ = birthday.get();
	bindWidget("birthday-setting",std::move(birthday));

	auto countryFlag = cpp14::make_unique<WImage>();
	countryFlag->addStyleClass("oj-country-flag");
	countryFlag_ = countryFlag.get();
	auto country = cpp14::make_unique<WComboBox>();
	country_ = country.get();
	country_->setModel(countrymodel_);
	country_->changed().connect( [=] { countryFlag_->setImageLink(cpp17::any_cast<std::string>(countrymodel_->data(countrymodel_->index(country_->currentIndex(),0),ItemDataRole::Decoration))); });
	country_->changed().connect( [=] { countryChanged_ = true; });
	bindWidget("country-flag",std::move(countryFlag));
	bindWidget("country-setting",std::move(country));

	auto institution = cpp14::make_unique<WLineEdit>();
	institution->changed().connect( [=] { institutionChanged_ = true; });
	institution_ = institution.get();
	bindWidget("institution-setting",std::move(institution));

	auto uvaid = cpp14::make_unique<WLineEdit>();
	uvaid->disable();
	uvaid_ = uvaid.get();
	bindWidget("uvaid-setting",std::move(uvaid));

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

	if(!emailChanged_ && !firstnameChanged_ && !lastnameChanged_ && !birthdayChanged_ && !countryChanged_ && !institutionChanged_) return;

	WStringStream strm ;

	strm << "The following data will be updated:<br/><br/>";
	strm << "<ul>";
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

ProfileWidget::SecurityWidget::SecurityWidget(Session *session, DBModel *dbmodel) : session_(session), dbmodel_(dbmodel) {

        setTemplateText(WString::tr("settings-security"));

        addFunction("block",&WTemplate::Functions::block);
        addFunction("tr",&WTemplate::Functions::tr);
        addFunction("id",&WTemplate::Functions::id);

        auto applyButton = cpp14::make_unique<WPushButton>("Apply");
        applyButton->addStyleClass("btn-primary");
        bindWidget("apply-button",std::move(applyButton));

        auto resetButton = cpp14::make_unique<WPushButton>("Reset");
        bindWidget("reset-button",std::move(resetButton));
}

void ProfileWidget::SecurityWidget::login(Auth::Login& login) {

}

void ProfileWidget::SecurityWidget::logout() {

}

ProfileWidget::NotificationsWidget::NotificationsWidget(Session *session, DBModel *dbmodel) : session_(session), dbmodel_(dbmodel) {

/*	addWidget(cpp14::make_unique<WText>("Notifications"));

	auto notificationsTable = addWidget(cpp14::make_unique<WTable>());

	notificationsTable->elementAt(0,0)->addWidget(cpp14::make_unique<WText>("eMail notifications"));

	notificationsTable->elementAt(1,0)->addWidget(cpp14::make_unique<WText>("Browser notifications"));

	notificationsTable->elementAt(2,0)->addWidget(cpp14::make_unique<WText>("App notifications"));
*/
}

void ProfileWidget::NotificationsWidget::login(Auth::Login& login) {

}

void ProfileWidget::NotificationsWidget::logout() {

}

ProfileWidget::EditorWidget::EditorWidget(Session *session, DBModel *dbmodel) : session_(session), dbmodel_(dbmodel) {

	setTemplateText(WString::tr("settings-codeeditor"));

	addFunction("block",&WTemplate::Functions::block);
        addFunction("tr",&WTemplate::Functions::tr);
        addFunction("id",&WTemplate::Functions::id);

        auto applyButton = cpp14::make_unique<WPushButton>("Apply");
        applyButton->addStyleClass("btn-primary");
        bindWidget("apply-button",std::move(applyButton));

        auto resetButton = cpp14::make_unique<WPushButton>("Reset");
        bindWidget("reset-button",std::move(resetButton));
}

void ProfileWidget::EditorWidget::login(Auth::Login& login) {

}

void ProfileWidget::EditorWidget::logout() {

}
