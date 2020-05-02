/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <fstream>
#include <algorithm>
#include <cctype>
#include <random>
#include <climits>
#include <functional>
#include <chrono>
#include <liboath/oath.h>
#include <Magick++.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WFont.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WMessageBox.h>
#include <Wt/WTable.h>
#include <Wt/WRadioButton.h>
#include <Wt/Utils.h>
#include <Wt/WMemoryResource.h>
#include "external/QrCode.h"
#include "viewmodels/CountryModel.h"
#include "datastore/SettingStore.h"
#include "datastore/UserStore.h"
#include "ProfileWidget.h"

using namespace Wt;
using namespace Magick;
using random_bytes_engine = std::independent_bits_engine<std::default_random_engine, CHAR_BIT, unsigned char>;

ProfileWidget::ProfileWidget(DataStore *dataStore, const std::shared_ptr<CountryModel> countrymodel, AuthWidget* authWidget) :
	dataStore_(dataStore), countrymodel_(countrymodel), authWidget_(authWidget) {

	mainLayout_ = setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout_->setContentsMargins(0,0,0,0);
	mainLayout_->setPreferredImplementation(LayoutImplementation::JavaScript);

	auto pageTitle = mainLayout_->addWidget(cpp14::make_unique<WText>("Profile"),0);
	pageTitle->addStyleClass("oj-pagetitle");

	mustLoginWidget_ = mainLayout_->addWidget(cpp14::make_unique<WText>("You are not logged in."),1);
}


void ProfileWidget::login(Auth::Login& login) {

	mustLoginWidget_->hide();

	mainWidget_ = mainLayout_->addWidget(cpp14::make_unique<WContainerWidget>(),1);
	auto menuLayout = mainWidget_->setLayout(cpp14::make_unique<WHBoxLayout>());
	menuLayout->setContentsMargins(0,0,0,0);
	menuLayout->setPreferredImplementation(LayoutImplementation::JavaScript);

	auto mainStack = menuLayout->addWidget(cpp14::make_unique<WStackedWidget>(),1);
	auto menuWidget = menuLayout->insertWidget(0,cpp14::make_unique<WMenu>(mainStack),0);
	menuWidget->addStyleClass("flex-column");
	menuWidget->setWidth(200);

	auto accountWidget = cpp14::make_unique<AccountWidget>(dataStore_->getUserStore(),countrymodel_);
	loginSignal().connect(accountWidget.get(),&AccountWidget::login);
	logoutSignal().connect(accountWidget.get(),&AccountWidget::logout);
	auto accountItem = menuWidget->addItem("Account",std::move(accountWidget));

	auto securityWidget = cpp14::make_unique<SecurityWidget>(dataStore_,authWidget_);
	loginSignal().connect(securityWidget.get(),&SecurityWidget::login);
	logoutSignal().connect(securityWidget.get(),&SecurityWidget::logout);
	auto securityItem = menuWidget->addItem("Security",std::move(securityWidget));

	auto notificationsWidget = cpp14::make_unique<NotificationsWidget>(dataStore_->getUserStore());
	loginSignal().connect(notificationsWidget.get(),&NotificationsWidget::login);
	logoutSignal().connect(notificationsWidget.get(),&NotificationsWidget::logout);
	auto notificationsItem = menuWidget->addItem("Notifications",std::move(notificationsWidget));

	loginSignal().emit(login);
}

void ProfileWidget::logout() {

	mainLayout_->removeWidget(mainWidget_);
	mustLoginWidget_->show();

	logoutSignal().emit();
}

ProfileWidget::AccountWidget::AccountWidget(UserStore *userStore, const std::shared_ptr<CountryModel> countrymodel) :
				userStore_(userStore), countrymodel_(countrymodel) {

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
	avatarFileUpload_ = avatarFileLayout->addWidget(cpp14::make_unique<WFileUpload>(),1);
	avatarFileUpload_->setFilters("image/x-png,image/jpeg");
	avatarFileUpload_->changed().connect(avatarFileUpload_,&WFileUpload::upload);
	avatarFileUpload_->uploaded().connect(this,&ProfileWidget::AccountWidget::avatarUploaded);
	avatarFileUpload_->fileTooLarge().connect( [=] {
		WMessageBox::show("Error","The file that you are trying to upload is too large.",StandardButton::Ok);
	});
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
		avatarImage_->setImageLink(userStore_->getAvatarLink(std::stoll(login_->user().id()),(AvatarType)avatarGroup->checkedId()));
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

	auto telegramUsername = cpp14::make_unique<WLineEdit>();
	telegramUsername->changed().connect( [=] {
		telegramUsernameChanged_ = true;
	});
	telegramUsername_ = bindWidget("telegramUsername-setting",std::move(telegramUsername));

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

	UserData userData = userStore_->getUserById(std::stoll(login_->user().id()));

	avatarGroup_->button((int)userData.avatarType)->setChecked();
	avatarImage_->setImageLink(userStore_->getAvatarLink(userData.id));
	if(userData.avatarType == AvatarType::Custom) {
		avatarUpload_->removeStyleClass("hidden");
	} else {
		avatarUpload_->addStyleClass("hidden");
	}
	avatarChanged_ = false;
	customAvatarChanged_ = false;
	username_->setText(userData.username);
	email_->setText(userData.email);
	emailChanged_ = false;
	telegramUsername_->setText("@Temp");
	telegramUsernameChanged_ = false;
	firstname_->setText(userData.firstname);
	firstnameChanged_ = false;
	lastname_->setText(userData.lastname);
	lastnameChanged_ = false;
	birthday_->setDate(userData.birthday);
	birthdayChanged_ = false;
	country_->setCurrentIndex(countrymodel_->match(countrymodel_->index(0,0),CountryModel::CountryCodeRole,userData.country,1,MatchFlag::Exactly|MatchFlag::Wrap).at(0).row());
	countryFlag_->setImageLink(cpp17::any_cast<std::string>(countrymodel_->data(countrymodel_->index(country_->currentIndex(),0),ItemDataRole::Decoration)));
	countryChanged_ = false;
	institution_->setText(userData.institution);
	institutionChanged_ = false;
	uvaid_->setText(userData.uvaID);
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

	if(!avatarChanged_ && !customAvatarChanged_ && !emailChanged_ && !firstnameChanged_ && !lastnameChanged_ && !birthdayChanged_ && !countryChanged_ && !institutionChanged_) return;

	WStringStream strm;

	strm << "The following data will be updated:<br/><br/>";
	strm << "<ul>";
	if(avatarChanged_) {
		if((AvatarType)avatarGroup_->checkedId() == AvatarType::Custom && !customAvatarChanged_) {
			strm << "<li>Avatar changed to: <b>You set the avatar type to 'Custom', but didn't upload a custom avatar. Change won't be saved.</b></li>";
		} else {
			strm << "<li>Avatar changed to: <b>" << avatarGroup_->checkedButton()->text().toUTF8() << "</b></li>";
		}
	}
	if(emailChanged_) strm << "<li>eMail address to: <b>" << email_->text().toUTF8() << "</b></li>";
	if(telegramUsernameChanged_) strm << "<li>Telegram Username to: <b>" << telegramUsername_->text().toUTF8() << "</b></li>";
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
			        if(avatarChanged_) {
			                if((AvatarType)avatarGroup_->checkedId() != AvatarType::Custom) {
						userStore_->setUserSetting(login_->user(),UserSettingType::AvatarType,(AvatarType)avatarGroup_->checkedId());
					} else if((AvatarType)avatarGroup_->checkedId() == AvatarType::Custom && customAvatarChanged_) {
						userStore_->setUserSetting(login_->user(),UserSettingType::Avatar,customAvatar_);
						userStore_->setUserSetting(login_->user(),UserSettingType::AvatarType,(AvatarType)avatarGroup_->checkedId());
					}
				}
			        if(emailChanged_) myAuthService.verifyEmailAddress(login_->user(),email_->text().toUTF8());
			        if(telegramUsernameChanged_);
			        if(firstnameChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::Firstname,firstname_->text().toUTF8());
				if(lastnameChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::Lastname,lastname_->text().toUTF8());
			        if(birthdayChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::Birthday,birthday_->date());
				if(countryChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::Country,countrymodel_->data(countrymodel_->index(country_->currentIndex(),0),
                                                                                                                                  CountryModel::CountryCodeRole));
				if(institutionChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::Institution, institution_->text().toUTF8());

			        avatarChanged_ = false;
			        customAvatarChanged_ = false;
			        emailChanged_ = false;
			        firstnameChanged_ = false;
			        lastnameChanged_ = false;
			        birthdayChanged_ = false;
			        countryChanged_ = false;
			        institutionChanged_ = false;
			}
			break;
		case StandardButton::No:
			break;
		}
		removeChild(warningBox);
	});
	warningBox->show();
}

void ProfileWidget::AccountWidget::avatarUploaded() {

	InitializeMagick(NULL);
	try {
		Image avatar(avatarFileUpload_->spoolFileName());
		if(avatar.magick() != "PNG" && avatar.magick() != "JPEG") {
			WMessageBox::show("Error","The image is not in JPEG or PNG format.",StandardButton::Ok);
		} else if(avatar.columns() != 128 || avatar.rows() !=128) {
			WMessageBox::show("Error","The image must have a size of 128x128 pixels.",StandardButton::Ok);
		} else {
			std::ifstream avatarFile(avatarFileUpload_->spoolFileName(), std::ios::binary);
			customAvatar_.assign(std::istreambuf_iterator<char>{avatarFile},{});
			std::string mimeType("image/" + std::string((avatar.magick()=="JPEG" ? "jpeg" : "png")));
			std::string base64data("data:"+mimeType+";base64,"+Utils::base64Encode(std::string(customAvatar_.begin(),customAvatar_.end()),false));
			avatarImage_->setImageLink(WLink(base64data));
			customAvatarChanged_ = true;
		}
	} catch(Exception &error_) {
		WMessageBox::show("Error","You are trying to upload a file of unknown format.",StandardButton::Ok);
	}

}

ProfileWidget::SecurityWidget::SecurityWidget(DataStore *dataStore, AuthWidget *authWidget) :
							dataStore_(dataStore), authWidget_(authWidget) {

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

	twofa_ = bindWidget("twofa-setting",cpp14::make_unique<WPushButton>());
	twofa_->clicked().connect( [=] {
		twofaDialog_ = addChild(cpp14::make_unique<TwoFADialog>(dataStore_,login_,disableTwoFA_));
		twofaDialog_->finished().connect(this,&ProfileWidget::SecurityWidget::TwoFADialogDone);
		twofaDialog_->show();
	});
}

void ProfileWidget::SecurityWidget::twofaButtonReset() {

	UserStore *userStore = dataStore_->getUserStore();

	if(cpp17::any_cast<bool>(userStore->getUserSetting(login_->user(),UserSettingType::TwoFAEnabled)) == true) {
		twofa_->setText("Disable 2FA");
		twofa_->removeStyleClass("btn-primary");
		twofa_->addStyleClass("btn-default");
		disableTwoFA_ = true;
	} else {
		twofa_->setText("Enable 2FA");
		twofa_->removeStyleClass("btn-default");
		twofa_->addStyleClass("btn-primary");
		disableTwoFA_ = false;
	}
}

void ProfileWidget::SecurityWidget::TwoFADialogDone(DialogCode code) {

	if(code == DialogCode::Accepted) {
		if(!disableTwoFA_) {
			std::vector<unsigned char> secret(twofaDialog_->getSecret());
			auto t = time(NULL);
			oath_init();
			int result = oath_totp_validate(
				reinterpret_cast<char*> (&secret[0]),
				32,
				t,
				OATH_TOTP_DEFAULT_TIME_STEP_SIZE,
				OATH_TOTP_DEFAULT_START_TIME,
				10,
				twofaDialog_->getAuthCode().c_str()
				);
			oath_done();

			if(result != OATH_INVALID_OTP) {
				UserStore *userStore = dataStore_->getUserStore();
				userStore->setUserSetting(login_->user(), UserSettingType::TwoFASecret, secret);
				userStore->setUserSetting(login_->user(), UserSettingType::TwoFAEnabled, true);
			} else {
				twofaDialog_->setCodeError();
				twofaDialog_->show();
				return;
			}
		} else {
			std::vector<unsigned char> secret(cpp17::any_cast<std::vector<unsigned char> >(dataStore_->getUserStore()->getUserSetting(login_->user(),UserSettingType::TwoFASecret)));

			auto t = time(NULL);
			oath_init();
			int result = oath_totp_validate(
				reinterpret_cast<char*> (&secret[0]),
				32,
				t,
				OATH_TOTP_DEFAULT_TIME_STEP_SIZE,
				OATH_TOTP_DEFAULT_START_TIME,
				10,
				twofaDialog_->getAuthCode().c_str()
				);
			oath_done();

			if(result != OATH_INVALID_OTP) {
				dataStore_->getUserStore()->setUserSetting(login_->user(),UserSettingType::TwoFAEnabled,false);
				dataStore_->getUserStore()->setUserSetting(login_->user(),UserSettingType::TwoFASecret,std::vector<unsigned char>());
			} else {
				twofaDialog_->setCodeError();
				twofaDialog_->show();
				return;
			}
		}
	}

	removeChild(twofaDialog_);
	twofaButtonReset();
}

void ProfileWidget::SecurityWidget::login(Auth::Login& login) {

	login_ = &login;
	twofaButtonReset();
}

void ProfileWidget::SecurityWidget::logout() {

}

ProfileWidget::SecurityWidget::TwoFADialog::TwoFADialog(DataStore *dataStore, Auth::Login *login, bool disable) : dataStore_(dataStore), login_(login), secret_(32) {

	setWindowTitle((disable ? "Disable" : "Enable") + std::string(" 2-Factor Authentication"));
	setClosable(true);
	auto mainLayout = contents()->setLayout(cpp14::make_unique<WVBoxLayout>());
	mainLayout->setContentsMargins(0,0,0,0);

	if(!disable) {
		auto f = [](unsigned char const c) {
				 return std::isspace(c);
			 };
		auto qrLayout = mainLayout->addLayout(cpp14::make_unique<WHBoxLayout>());
		qrLayout->setContentsMargins(0,0,0,0);
		qrLayout->addStretch(1);
		auto qrCode = qrLayout->addWidget(cpp14::make_unique<WImage>());
		qrCode->setHeight(250);
		qrLayout->addStretch(1);

		std::string qrstr;
		qrstr += "otpauth://totp/";
		std::string sitetitle(dataStore_->getSettingStore()->getSetting("sitetitle"));
		sitetitle.erase(std::remove_if(sitetitle.begin(), sitetitle.end(), f), sitetitle.end());
		qrstr += sitetitle;
		qrstr += ":";
		qrstr += login_->user().identity("loginname").toUTF8();

		random_bytes_engine rbe;
		rbe.seed(std::chrono::system_clock::now().time_since_epoch().count());
		std::generate(begin(secret_), end(secret_), std::ref(rbe));
		char *base32secret;
		size_t base32len;
		oath_init();
		oath_base32_encode(reinterpret_cast<char*> (&secret_[0]),32,&base32secret,&base32len);
		oath_done();

		qrstr += "?secret=";
		qrstr += base32secret;

		qrstr += "&algorithm=SHA1&digits=6&period=30";

		// In case we want to add a logo
		// qrstr += "&image=<logo_escaped_url>";

		qrcodegen::QrCode twofaqr = qrcodegen::QrCode::encodeText(qrstr.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);
		std::string twofaqrsvg = twofaqr.toSvgString(4);

		qrCode->setImageLink(WLink(std::make_shared<WMemoryResource>("image/svg+xml",std::vector<unsigned char>(twofaqrsvg.begin(), twofaqrsvg.end()))));

		mainLayout->addWidget(cpp14::make_unique<WText>("Scan the above QR code with your preferred 2FA app and enter the resulting code below"));
	} else {
		mainLayout->addWidget(cpp14::make_unique<WText>("Enter the current 2FA code from your app to disable it"));
	}

	auto codeLayout = mainLayout->addLayout(cpp14::make_unique<WHBoxLayout>());
	codeLayout->setContentsMargins(0,0,0,0);
	codeLayout->addStretch(1);
	authCode_ = codeLayout->addWidget(cpp14::make_unique<WLineEdit>());
	authCode_->setWidth(200);
	authCode_->setInputMask("999999");
	codeLayout->addStretch(1);

	WPushButton *ok = footer()->addWidget(cpp14::make_unique<WPushButton>("Ok"));
	ok->addStyleClass("btn-primary");
	WPushButton *cancel = footer()->addWidget(cpp14::make_unique<WPushButton>("Cancel"));

	ok->clicked().connect(this,&WDialog::accept);
	cancel->clicked().connect(this,&WDialog::reject);
}

std::vector<unsigned char> ProfileWidget::SecurityWidget::TwoFADialog::getSecret() {
	return secret_;
}

std::string ProfileWidget::SecurityWidget::TwoFADialog::getAuthCode() {

	return authCode_->text().toUTF8();
}

void ProfileWidget::SecurityWidget::TwoFADialog::setCodeError() {
	authCode_->addStyleClass("has-error");
}

ProfileWidget::NotificationsWidget::NotificationsWidget(UserStore *userStore) : userStore_(userStore) {

	setTemplateText(WString::tr("settings-notifications"));

	addFunction("block",&WTemplate::Functions::block);
	addFunction("tr",&WTemplate::Functions::tr);
	addFunction("id",&WTemplate::Functions::id);

	auto notificationsTable = cpp14::make_unique<WTable>();
	notificationsTable->addStyleClass("oj-settings-notifications");

	notificationsTable->elementAt(0,1)->addWidget(cpp14::make_unique<WText>("eMail"));
	notificationsTable->elementAt(0,2)->addWidget(cpp14::make_unique<WText>("Browser"));
	notificationsTable->elementAt(0,3)->addWidget(cpp14::make_unique<WText>("Telegram"));

	notificationsTable->elementAt(1,0)->addWidget(cpp14::make_unique<WText>("Submission results"));
	notificationsTable->elementAt(2,0)->addWidget(cpp14::make_unique<WText>("Contest announcements"));
	notificationsTable->elementAt(3,0)->addWidget(cpp14::make_unique<WText>("General information"));
	notificationsTable->elementAt(4,0)->addWidget(cpp14::make_unique<WText>("Messages"));

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
	emailMessages_ = notificationsTable->elementAt(4,1)->addWidget(cpp14::make_unique<WCheckBox>());
	emailGeneral_->changed().connect( [=] {
		emailMessagesChanged_ = true;
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
	browserMessages_ = notificationsTable->elementAt(4,2)->addWidget(cpp14::make_unique<WCheckBox>());
	browserGeneral_->changed().connect( [=] {
		browserMessagesChanged_ = true;
	});

	telegramResults_ = notificationsTable->elementAt(1,3)->addWidget(cpp14::make_unique<WCheckBox>());
	telegramResults_->changed().connect( [=] {
		telegramResultsChanged_ = true;
	});
	telegramContests_ = notificationsTable->elementAt(2,3)->addWidget(cpp14::make_unique<WCheckBox>());
	telegramContests_->changed().connect( [=] {
		telegramContestsChanged_ = true;
	});
	telegramGeneral_ = notificationsTable->elementAt(3,3)->addWidget(cpp14::make_unique<WCheckBox>());
	telegramGeneral_->changed().connect( [=] {
		telegramGeneralChanged_ = true;
	});
	telegramMessages_ = notificationsTable->elementAt(4,3)->addWidget(cpp14::make_unique<WCheckBox>());
	telegramGeneral_->changed().connect( [=] {
		telegramMessagesChanged_ = true;
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

	emailResults_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsEmailResults)) ? CheckState::Checked : CheckState::Unchecked);
	emailContests_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsEmailContests)) ? CheckState::Checked : CheckState::Unchecked);
	emailGeneral_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsEmailGeneral)) ? CheckState::Checked : CheckState::Unchecked);
	emailMessages_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsEmailMessages)) ? CheckState::Checked : CheckState::Unchecked);
	
	browserResults_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsBrowserResults)) ? CheckState::Checked : CheckState::Unchecked);
	browserContests_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsBrowserContests)) ? CheckState::Checked : CheckState::Unchecked);
	browserGeneral_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsBrowserGeneral)) ? CheckState::Checked : CheckState::Unchecked);
	browserMessages_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsBrowserMessages)) ? CheckState::Checked : CheckState::Unchecked);
	
	telegramResults_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsTelegramResults)) ? CheckState::Checked : CheckState::Unchecked);
	telegramContests_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsTelegramContests)) ? CheckState::Checked : CheckState::Unchecked);
	telegramGeneral_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsTelegramGeneral)) ? CheckState::Checked : CheckState::Unchecked);
	telegramMessages_->setCheckState(cpp17::any_cast<bool>(userStore_->getUserSetting(login_->user(), UserSettingType::NotificationsTelegramMessages)) ? CheckState::Checked : CheckState::Unchecked);
	
	emailResultsChanged_ = false;
	emailContestsChanged_ = false;
	emailGeneralChanged_ = false;
	browserResultsChanged_ = false;
	browserContestsChanged_ = false;
	browserGeneralChanged_ = false;
	emailResultsChanged_ = false;
	emailContestsChanged_ = false;
	emailGeneralChanged_ = false;
	emailMessagesChanged_ = false;
	browserResultsChanged_ = false;
	browserContestsChanged_ = false;
	browserGeneralChanged_ = false;
	browserMessagesChanged_ = false;
	telegramResultsChanged_ = false;
	telegramContestsChanged_ = false;
	telegramGeneralChanged_ = false;
	telegramMessagesChanged_ = false;
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

	if(!emailResultsChanged_ && !emailContestsChanged_ && !emailGeneralChanged_ && !emailMessagesChanged_ &&
	   !browserResultsChanged_ && !browserContestsChanged_ && !browserGeneralChanged_ && !browserMessagesChanged_ &&
	   !telegramResultsChanged_ && !telegramContestsChanged_ && !telegramGeneralChanged_ && !telegramMessagesChanged_) return;

	WStringStream strm;

	strm << "The following data will be updated:<br/><br/>";
	strm << "<ul>";
	if(emailResultsChanged_) strm << "<li>eMail submission results to: <b>" << (emailResults_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(emailContestsChanged_) strm << "<li>eMail contests announcements to: <b>" << (emailContests_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(emailGeneralChanged_) strm << "<li>eMail general information to: <b>" << (emailGeneral_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(emailMessagesChanged_) strm << "<li>eMail messages to: <b>" << (emailMessages_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(browserResultsChanged_) strm << "<li>Browser submission results to: <b>" << (browserResults_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(browserContestsChanged_) strm << "<li>Browser contests announcements to: <b>" << (browserContests_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(browserGeneralChanged_) strm << "<li>Browser general information to: <b>" << (browserGeneral_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(browserMessagesChanged_) strm << "<li>Browser messages to: <b>" << (browserMessages_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(telegramResultsChanged_) strm << "<li>Telegram submission results to: <b>" << (telegramResults_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(telegramContestsChanged_) strm << "<li>Telegram contests announcements to: <b>" << (telegramContests_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(telegramGeneralChanged_) strm << "<li>Telegram general information to: <b>" << (telegramGeneral_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	if(telegramMessagesChanged_) strm << "<li>Telegram messages to: <b>" << (telegramMessages_->isChecked() ? "enabled" : "disabled") << "</b></li>";
	strm << "</ul>";
	strm << "<br/>Do you want to continue?";

	auto warningBox = addChild(cpp14::make_unique<WMessageBox>("Are you sure?","",Icon::Information,StandardButton::Yes | StandardButton::No));
	warningBox->textWidget()->setTextFormat(TextFormat::XHTML);
	warningBox->setText(strm.str());

	warningBox->buttonClicked().connect( [=] (StandardButton button) {
		switch(button) {
		case StandardButton::Yes:
			{
				if(emailResultsChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsEmailResults,(emailResults_->isChecked() ? true : false));
				if(emailContestsChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsEmailContests,(emailContests_->isChecked() ? true : false));
				if(emailGeneralChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsEmailGeneral,(emailGeneral_->isChecked() ? true : false));
				if(emailMessagesChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsEmailMessages,(emailMessages_->isChecked() ? true : false));

				if(browserResultsChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsBrowserResults,(browserResults_->isChecked() ? true : false));
				if(browserContestsChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsBrowserContests,(browserContests_->isChecked() ? true : false));
				if(browserGeneralChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsBrowserGeneral,(browserGeneral_->isChecked() ? true : false));
				if(browserMessagesChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsBrowserMessages,(browserMessages_->isChecked() ? true : false));

				if(telegramResultsChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsTelegramResults,(telegramResults_->isChecked() ? true : false));
				if(telegramContestsChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsTelegramContests,(telegramContests_->isChecked() ? true : false));
				if(telegramGeneralChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsTelegramGeneral,(telegramGeneral_->isChecked() ? true : false));
				if(telegramMessagesChanged_) userStore_->setUserSetting(login_->user(),UserSettingType::NotificationsTelegramMessages,(telegramMessages_->isChecked() ? true : false));

			        emailResultsChanged_ = false;
			        emailContestsChanged_ = false;
			        emailGeneralChanged_ = false;
			        emailMessagesChanged_ = false;
			        browserResultsChanged_ = false;
			        browserContestsChanged_ = false;
			        browserGeneralChanged_ = false;
			        browserMessagesChanged_ = false;
			        telegramResultsChanged_ = false;
			        telegramContestsChanged_ = false;
			        telegramGeneralChanged_ = false;
			        telegramMessagesChanged_ = false;
			}
			break;
		case StandardButton::No:
			break;
		}
		removeChild(warningBox);
	});
	warningBox->show();
}
