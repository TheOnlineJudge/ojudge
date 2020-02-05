/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#include <Wt/Dbo/backend/Postgres.h>
#include "DBModel.h"

DBModel::DBModel(const std::string &postgresDb) {
	std::unique_ptr<dbo::backend::Postgres> connection{new dbo::backend::Postgres(postgresDb)};
        connection->setProperty("show-queries", "true");
        session.setConnection(std::move(connection));

	session.mapClass<Category>("category");
	session.mapClass<Problem>("problem");
	session.mapClass<Testcase>("testcase");
	session.mapClass<Setting>("setting");
	session.mapClass<Submission>("submission");
	session.mapClass<Verdict>("verdict");

        try {
                session.createTables();
                std::cerr << "Created database." << std::endl;
		{	dbo::Transaction transaction(session) ;

			// Setting some default values in the database
			std::unique_ptr<Category> category(new Category());
			category->title = "Root";
			category->order = 0;
			session.add(std::move(category));

			std::vector< std::pair<std::string,std::string> > defaultSettings;
			
			defaultSettings.push_back(std::make_pair("siteTitle","OJudge"));
			defaultSettings.push_back(std::make_pair("siteLogo", "images/logo.svg"));
			defaultSettings.push_back(std::make_pair("siteColor", "#337ab7"));
			defaultSettings.push_back(std::make_pair("googleAnalytics", ""));

			for(const std::pair<std::string, std::string> &i : defaultSettings) {
				std::unique_ptr<Setting> setting(new Setting());
				setting->settingName = i.first;
				setting->settingValue = i.second;
				session.add(std::move(setting));
			}
		}
        } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "Using existing database." << std::endl;
        }
}

DBModel::~DBModel() {

}

dbo::Transaction DBModel::startTransaction() {
	return dbo::Transaction(session) ;
}

dbo::ptr<Category> DBModel::addCategory(std::string title, int parent) {

	dbo::Transaction transaction(session) ;
	std::unique_ptr<Category> category(new Category());
	category->title = title ;
	category->parent = session.load<Category>(parent);
	category->order = category->parent->children.size();

	return session.add(std::move(category));
}

Categories DBModel::getCategories() {

	dbo::Transaction transaction(session) ;
	return session.find<Category>().orderBy("parent_id,\"order\"");
}

Settings DBModel::getSettings() {

	dbo::Transaction transaction(session);
	return session.find<Setting>();
}

std::string DBModel::getSetting(std::string settingName) {

	dbo::Transaction transaction(session);
	Dbo::ptr<Setting> setting = session.find<Setting>().where("settingname = ?").bind(settingName);
	return setting->settingValue;
}
