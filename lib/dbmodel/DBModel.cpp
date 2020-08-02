/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/Auth/HashFunction.h>
#include <Wt/Auth/PasswordStrengthValidator.h>
#include <Wt/Auth/PasswordVerifier.h>
#include <Wt/Auth/Dbo/AuthInfo.h>
#include <Wt/Auth/Dbo/UserDatabase.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/Dbo/Impl.h>
#include <Wt/Utils.h>

#include "DBModel.h"
#include "../Joomla10HashFunction.h"

void Session::configureAuth() {
	myAuthService.setAuthTokensEnabled(true, "logincookie");
	myAuthService.setEmailVerificationEnabled(true);
	myAuthService.setEmailVerificationRequired(true);

	auto verifier = cpp14::make_unique<Auth::PasswordVerifier>();
	verifier->addHashFunction(cpp14::make_unique<Auth::BCryptHashFunction>(7));
	verifier->addHashFunction(cpp14::make_unique<Joomla10HashFunction>());
	myPasswordService.setVerifier(std::move(verifier));
	myPasswordService.setAttemptThrottlingEnabled(true);
	myPasswordService.setStrengthValidator(cpp14::make_unique<Auth::PasswordStrengthValidator>());

	if (Auth::GoogleService::configured()) {
		myOAuthServices.push_back(cpp14::make_unique<Auth::GoogleService>(myAuthService));
	}

	if (Auth::FacebookService::configured()) {
		myOAuthServices.push_back(cpp14::make_unique<Auth::FacebookService>(myAuthService));
	}
}

Session::Session(const std::string &postgresDb) {
	auto connection = cpp14::make_unique<Dbo::backend::Postgres>(postgresDb);
	connection->setProperty("show-queries", "true");
	setConnection(std::move(connection));
}

Session::~Session() {

}

void Session::loadUsers() {
	users_ = cpp14::make_unique<UserDatabase>(*this);
}

void Session::createUserData(const Auth::User &newUser) {

	dbo::Transaction t(*this);

	dbo::ptr<User> userData = user(newUser);
	userData.modify()->role = Role::Registered;
	dbo::ptr<UserSettings> settings  = add(std::unique_ptr<UserSettings>(new UserSettings()));
	dbo::ptr<UserAvatar> avatar = add(std::unique_ptr<UserAvatar>(new UserAvatar()));
	avatar.modify()->avatarType = AvatarType::Default;
	userData.modify()->settings = settings;
	userData.modify()->avatar = avatar;
}

Auth::AbstractUserDatabase &Session::users() {
	return *users_;
}

dbo::ptr<User> Session::user() {
	if(login_.loggedIn()) {
		return user(login_.user());
	} else {
		return dbo::ptr<User>();
	}
}

dbo::ptr<User> Session::user(const Auth::User &authUser) {
	dbo::ptr<AuthInfo> authInfo = users_->find(authUser);

	dbo::ptr<User> user = authInfo->user();

	if(!user) {
		user = add(Wt::cpp14::make_unique<User>());
		authInfo.modify()->setUser(user);
	}

	return user;
}

const Auth::AuthService &Session::auth() {
	return myAuthService;
}

const Auth::PasswordService &Session::passwordAuth() {
	return myPasswordService;
}

const std::vector<const Auth::OAuthService *> Session::oAuth() {
	std::vector<const Auth::OAuthService *> result;
	for(auto &auth : myOAuthServices) {
		result.push_back(auth.get());
	}
	return result;
}

DBModel::DBModel(Session* session) : session_(session) {

	session_->mapClass<User>("user");
	session_->mapClass<UserSettings>("user_settings");
	session_->mapClass<UserAvatar>("user_avatar");
	session_->mapClass<Notification>("notification");
	session_->mapClass<Message>("message");
	session_->mapClass<Category>("category");
	session_->mapClass<Problem>("problem");
	session_->mapClass<ProblemAuthor>("problem_author");
	session_->mapClass<ProblemSource>("problem_source");
	session_->mapClass<ProblemRating>("problem_rating");
	session_->mapClass<ProblemBookmark>("problem_bookmark");
	session_->mapClass<Description>("description");
	session_->mapClass<Testcase>("testcase");
	session_->mapClass<Setting>("setting");
	session_->mapClass<Submission>("submission");
	session_->mapClass<Verdict>("verdict");
	session_->mapClass<VerdictResult>("verdict_result");
	session_->mapClass<Language>("language");
	session_->mapClass<Contest>("contest");
	session_->mapClass<AuthInfo>("auth_info");
	session_->mapClass<AuthInfo::AuthIdentityType>("auth_identity");
	session_->mapClass<AuthInfo::AuthTokenType>("auth_token");

	bool firstRun = false;
	Auth::User newUser;

	try {
		session_->createTables();
		std::cerr << "Created database." << std::endl;
		{       dbo::Transaction t = startTransaction();

			// Setting some default values in the database
			std::unique_ptr<Category> category(new Category());
			category->title = "Root";
			category->order = 0;
			session_->add(std::move(category));

			std::vector< std::pair<std::string,std::string> > defaultSettings;

			defaultSettings.push_back(std::make_pair("sitetitle","OJudge"));
			defaultSettings.push_back(std::make_pair("sitelogo", "images/logo.svg"));
			defaultSettings.push_back(std::make_pair("sitecolor", "#0055ff"));
			defaultSettings.push_back(std::make_pair("googleanalytics", ""));

			for(const std::pair<std::string, std::string> &i : defaultSettings) {
				std::unique_ptr<Setting> setting(new Setting());
				setting->settingName = i.first;
				setting->settingValue = i.second;
				session_->add(std::move(setting));
			}

			// Load all the possible verdicts
			std::map<long long, std::pair<std::string, std::string> > defaultVerdicts;
	
			defaultVerdicts[10] = std::make_pair("Submission Error","SE");
			defaultVerdicts[15] = std::make_pair("Can't be judged","CJ");
			defaultVerdicts[20] = std::make_pair("In Queue","QU");
			defaultVerdicts[30] = std::make_pair("Compilation Error","CE");
			defaultVerdicts[35] = std::make_pair("Restricted Function","RF");
			defaultVerdicts[40] = std::make_pair("Runtime Error","RE");
			defaultVerdicts[45] = std::make_pair("Output Limit Exceeded","OL");
			defaultVerdicts[50] = std::make_pair("Time Limit Exceeded","TL");
			defaultVerdicts[60] = std::make_pair("Memory Limit Exceeded","ML");
			defaultVerdicts[65] = std::make_pair("Broken Communication","BC");
			defaultVerdicts[66] = std::make_pair("Protocol Violation","PV");
			defaultVerdicts[67] = std::make_pair("Protocol Limit Exceeded","PL");
			defaultVerdicts[70] = std::make_pair("Wrong Answer","WA");
			defaultVerdicts[80] = std::make_pair("Presentation Error","PE");
			defaultVerdicts[90] = std::make_pair("Accepted","AC");

			for(const std::pair<long long, std::pair<std::string, std::string> > &i : defaultVerdicts) {
				std::unique_ptr<VerdictResult> vr(new VerdictResult());
				vr->id = i.first;
				vr->longname = i.second.first;
				vr->shortname = i.second.second;
				session_->add(std::move(vr));
			}

			// Add username admin with password admin
			std::unique_ptr<UserDatabase> tmpUsers = cpp14::make_unique<UserDatabase>(*session_);
			std::cerr << "Creating user 'admin' with password 'admin' on first run" << std::endl;
			newUser = tmpUsers->registerNew();
			newUser.addIdentity(Auth::Identity::LoginName, "admin");
			newUser.setEmail("dummy@address.here");
			firstRun = true;}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "Using existing database." << std::endl;
	}

	session_->loadUsers();

	if(firstRun) {
		dbo::Transaction t = startTransaction();
		dbo::ptr<User> userData = session_->user(newUser);
		userData.modify()->role = Role::Admin;
		dbo::ptr<UserSettings> settings = session_->add(std::unique_ptr<UserSettings>(new UserSettings()));
		dbo::ptr<UserAvatar> avatar = session_->add(std::unique_ptr<UserAvatar>(new UserAvatar()));
		avatar.modify()->avatarType = AvatarType::Default;
		userData.modify()->settings = settings;
		userData.modify()->avatar = avatar;
		session_->passwordAuth().updatePassword(newUser,"admin");
	}
}

DBModel::~DBModel() {

}

dbo::Transaction DBModel::startTransaction() {
	return dbo::Transaction(*session_);
}

Categories DBModel::getCategories() {

	dbo::Transaction t = startTransaction();
	return session_->find<Category>().orderBy("parent_id,\"order\"");
}

dbo::ptr<Category> DBModel::addCategory(std::string title, int parent) {

	dbo::Transaction t = startTransaction();
	std::unique_ptr<Category> category(new Category());
	category->title = title;
	category->parent = session_->load<Category>(parent);
	category->order = category->parent->children.size();

	return session_->add(std::move(category));
}

Problems DBModel::getProblems() {

	dbo::Transaction t = startTransaction();
	return session_->find<Problem>().orderBy("id");
}

Users DBModel::getUsers() {

	dbo::Transaction t = startTransaction();
	return session_->find<User>().orderBy("id");
}

dbo::ptr<User> DBModel::getUser(long long id) {

	dbo::Transaction t = startTransaction();
	return session_->find<User>().where("id = ?").bind(id);
}

dbo::ptr<User> DBModel::getUser(const Auth::User& user) {

	dbo::Transaction t = startTransaction();
	return session_->user(user);
}

dbo::ptr<Problem> DBModel::getProblem(long long id) {

	dbo::Transaction t = startTransaction();
	return session_->find<Problem>().where("id = ?").bind(id);
}

dbo::ptr<Problem> DBModel::addProblem(long long id, std::string title) {

	dbo::Transaction t = startTransaction();
	auto problem = std::unique_ptr<Problem>(new Problem());
	problem->id = id;
	problem->title = title;
	auto problemRet = session_->add(std::move(problem));
	problemRet.modify()->description = session_->add(std::unique_ptr<Description>(new Description()));

	return problemRet;
}

void DBModel::setProblemCategories(long long id, std::set<int> categories) {

	dbo::Transaction t = startTransaction();
	dbo::ptr<Problem> problem = session_->find<Problem>().where("id = ?").bind(id);
	problem.modify()->categories.clear();

	for(auto tmpCatId: categories) {
		dbo::ptr<Category> tmpCat = session_->find<Category>().where("id = ?").bind(tmpCatId);
		problem.modify()->categories.insert(tmpCat);
	}
}

void DBModel::updateHtmlDescription(long long problemId, std::optional<std::string> content) {

	dbo::Transaction t = startTransaction();
	dbo::ptr<Problem> problem = session_->find<Problem>().where("id = ?").bind(problemId);
	dbo::ptr<Description> description = problem->description;

	description.modify()->htmldata = content;
}

void DBModel::updatePdfDescription(long long problemId, std::optional<std::vector<unsigned char> > content) {

	dbo::Transaction t = startTransaction();
	dbo::ptr<Problem> problem = session_->find<Problem>().where("id = ?").bind(problemId);
	dbo::ptr<Description> description = problem->description;

	description.modify()->pdfdata = content;
}

const Settings DBModel::getSiteSettings() {
	dbo::Transaction t = startTransaction();
	return session_->find<Setting>();
}

void DBModel::updateSiteSetting(std::string settingName, std::string settingValue) {
	dbo::Transaction t = startTransaction();
	dbo::ptr<Setting> setting = session_->find<Setting>().where("settingname = ?").bind(settingName);
	setting.modify()->settingValue = settingValue;
}

dbo::ptr<UserSettings> DBModel::getUserSettings(const Auth::User& user) {
	dbo::Transaction t = startTransaction();
	dbo::ptr<User> userData = session_->user(user);
	return userData->settings;
}

dbo::ptr<Language> DBModel::addLanguage(std::unordered_map<std::string, cpp17::any> args){
	
	bool mandatoryArgsNotNull = true;
	mandatoryArgsNotNull = mandatoryArgsNotNull && args.find("name") != args.end(); 
	mandatoryArgsNotNull = mandatoryArgsNotNull && args.find("compilerVersion") != args.end(); 
	mandatoryArgsNotNull = mandatoryArgsNotNull && args.find("runScript") != args.end(); 
	//if (!mandatoryArgsNotNull) return ;

	auto language = std::unique_ptr<Language>(new Language());
	
	std::cerr << "A\n" << std::endl;
	language->name = cpp17::any_cast<std::string>(args["name"]);
	std::cerr << "A\n" << std::endl;
	language->compilerVersion = cpp17::any_cast<std::string>(args["compilerVersion"]);
	std::cerr << "A\n" << std::endl;
	language->runScript = cpp17::any_cast<std::vector<unsigned char>>(args["runScript"]);
	std::cerr << "A\n" << std::endl;
	
	if(args.find("description") != args.end())
		language->description = cpp17::any_cast<std::string>(args["description"]);
	if(args.find("codeSkeleton") != args.end())
		language->codeSkeleton = cpp17::any_cast<std::vector<unsigned char>>(args["codeSkeleton"]);
	if(args.find("aceStyle") != args.end())
		language->aceStyle = cpp17::any_cast<std::string>(args["aceStyle"]);
	if(args.find("compileScript") != args.end())
		language->compileScript = cpp17::any_cast<std::vector<unsigned char>>(args["compileScript"]);
	if(args.find("linkScript") != args.end())
		language->linkScript = cpp17::any_cast<std::vector<unsigned char>>(args["linkScript"]);
	
	std::cerr << "A\n" << std::endl;
	dbo::Transaction t = startTransaction();
	auto languageRet = session_->add(std::move(language));

	if(args.find("submissions") != args.end())
	{
		for (const dbo::ptr<Submission> &submission : std::any_cast<Submissions>(args["submissions"])){
			languageRet.modify()->submissions.insert(submission);
		}
	}

	if(args.find("contests") != args.end())
	{
		for (const dbo::ptr<Contest> &contest : std::any_cast<Contests>(args["contests"])){
			languageRet.modify()->contests.insert(contest);
		}
	}

	return languageRet;
}

Languages DBModel::getLanguages() {

	dbo::Transaction t = startTransaction();
	return session_->find<Language>();
}

VerdictResults DBModel::getVerdictResults() {

	dbo::Transaction t = startTransaction();
	return session_->find<VerdictResult>();
}
