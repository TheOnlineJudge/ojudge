/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WServer.h>
#include <Wt/WApplication.h>
#include <mutex>
#include "../ojudgeApp.h"

#include "../dbmodel/DBModel.h"

#include "LanguageStore.h"

std::mutex addLanguage_mutex;

using namespace Wt;

LanguageStore::LanguageStore(DBModel *dbModel) : dbModel_(dbModel) {

        Languages languages = dbModel_->getLanguages();
        Dbo::Transaction transaction = dbModel_->startTransaction();

	int row = 0;

        for(Languages::const_iterator i = languages.begin(); i != languages.end(); i++) {
                dbo::ptr<Language> language = *i;
		languageData_[row].id = language.id();
                languageData_[row].name = language->name;
		languageData_[row].compilerVersion = language->compilerVersion;
		languageData_[row].codeSkeleton = language->codeSkeleton;
		languageData_[row].compileScript = language->compileScript;
		languageData_[row].linkScript = language->linkScript;
		languageData_[row].runScript = language->runScript;
		row++;
        }
}

const std::map<long long,LanguageData>& LanguageStore::getLanguages() {
        return languageData_;
}

const LanguageData& LanguageStore::getLanguage(long long id) {
	return languageData_.at(id);
}

/*
void LanguageStore::addLanguage(long long id, std::string title, const WModelIndex& parent) {

	std::lock_guard<std::mutex> guard(addLanguage_mutex);

	dbo::ptr<Problem> problem = dbModel_->addProblem(id,title);

	ProblemData tmpProblemData;
	tmpProblemData.id = id;
	tmpProblemData.title = title;

	int row = problemData_.size();
	problemData_[row] = tmpProblemData;

	auto server = Wt::WServer::instance();
        server->postAll([=]{
                auto app = dynamic_cast<ojudgeApp*>(Wt::WApplication::instance());
                assert(app != nullptr);
                app->getViewModels()->getProblemModel()->insertProblem(row,parent);
                app->triggerUpdate();
        });
}
*/
