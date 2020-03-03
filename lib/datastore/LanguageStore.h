/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef LANGUAGESTORE_H
#define LANGUAGESTORE_H

#include <string>
#include <map>
#include <Wt/WModelIndex.h>

class DBModel;

struct LanguageData {
        long long id;
        std::string name;
        std::string compilerVersion;
	std::optional< std::vector<unsigned char> > codeSkeleton;
	std::optional< std::vector<unsigned char> > compileScript;
	std::optional< std::vector<unsigned char> > linkScript;
	std::vector<unsigned char> runScript;
};


class LanguageStore {
public:
LanguageStore(DBModel *dbModel);
const std::map<long long,LanguageData>& getLanguages();
const LanguageData& getLanguage(long long id);
//void addLanguage(long long id, std::string title, const Wt::WModelIndex& parent);

private:
DBModel *dbModel_;
std::map<long long,LanguageData> languageData_;
};

#endif // LANGUAGESTORE_H
