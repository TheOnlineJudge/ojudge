/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef COUNTRYSTORE_H
#define COUNTRYSTORE_H

#include<unordered_set>

class CountryStore {
public:
CountryStore();
const std::unordered_set<std::string> countryCodes();

private:
std::unordered_set<std::string> countryCodes_;
};

#endif // COUNTRYSTORE_H
