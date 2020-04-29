/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef SETTINGSSTORE_H
#define SETTINGSSTORE_H

#include <string>
#include <map>
#include "../dbmodel/DBModel.h"

class SettingStore {
public:
SettingStore(DBModel *dbModel);
const std::string getSetting(std::string setting);
bool setSetting(std::string setting, std::string value);

private:
DBModel *dbModel_;
std::map<std::string,std::string> settingData_;
};

#endif // SETTINGSSTORE_H
