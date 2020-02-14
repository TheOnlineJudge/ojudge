/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <sstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <Wt/WServer.h>
#include "lib/ojudgeApp.h"
#include "lib/authmodel/Session.h"
#include "lib/dbmodel/DBModel.h"
#include "lib/viewmodels/ViewModels.h"

using namespace Wt;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

po::variables_map vm;

std::unique_ptr<WApplication> createApplication(const WEnvironment &env) {

	std::stringstream dbConnect;
	dbConnect << "host=" << vm["database.host"].as<std::string>() << " ";
	dbConnect << "dbname=" << vm["database.dbname"].as<std::string>() << " ";
	dbConnect << "user=" << vm["database.user"].as<std::string>() << " ";
	dbConnect << "password=" << vm["database.password"].as<std::string>();

	Session *session = new Session(dbConnect.str().c_str());
	DBModel *dbmodel = new DBModel(dbConnect.str().c_str());
	ViewModels *viewModels = new ViewModels(dbmodel);

	return cpp14::make_unique<ojudgeApp>(env,session,viewModels,dbmodel);
}

int main(int argc, char **argv) {

	struct passwd *pw = getpwuid(getuid());
	std::stringstream configDirStr;
	std::stringstream configFilePathStr;
	std::string configDir;
	std::string configFilePath;

	configDirStr << pw->pw_dir << "/.config/ojudge/";
	configDir = configDirStr.str();
	configFilePathStr << configDir << "config";
	configFilePath = configFilePathStr.str();

	if(fs::exists(configFilePath)) {
		std::cout << "Config file is: " << configFilePath << std::endl;
	} else {
		std::cout << "Config file IS NOT present, creating skeleton" << std::endl;
		fs::create_directories(configDir);
		fs::ofstream configFile(configFilePath);
		configFile << "[database]" << std::endl;
		configFile << "host=localhost" << std::endl;
		configFile << "dbname=dbname" << std::endl;
		configFile << "user=user" << std::endl;
		configFile << "password=password" << std::endl;
		configFile.close();
		std::cout << "Please modify the file " << configFilePath << " with your actual configuration and run again" << std::endl;
		return 1;
	}

	po::options_description desc;
	desc.add_options()
	        ("database.host",po::value<std::string>(),"Hostname of the database")
	        ("database.dbname",po::value<std::string>(),"Database name")
	        ("database.user",po::value<std::string>(),"Database username")
	        ("database.password",po::value<std::string>(),"Database password")
	;
	po::store(po::parse_config_file(configFilePath.c_str(),desc),vm);

	try {
		WServer server(argc, argv, WTHTTP_CONFIGURATION);
		server.addEntryPoint(EntryPointType::Application, createApplication);

		Session::configureAuth();

		server.run();
	} catch (WServer::Exception &e) {
		std::cerr << e.what() << std::endl;
	} catch (std::exception &e) {
		std::cerr << "exception: " << e.what() << std::endl;
	}
}
