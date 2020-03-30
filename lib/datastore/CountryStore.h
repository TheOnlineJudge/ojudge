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
#include<Wt/WMemoryResource.h>

class CountryStore {
public:
CountryStore();
const std::vector<std::string> countryCodes();
const std::string getCountry(const int row);
const std::string countryName(const std::string& countryCode);
const std::shared_ptr<Wt::WMemoryResource> countryFlag(const std::string& countryCode);

private:
std::vector<std::string> countryCodes_;
};

#endif // COUNTRYSTORE_H
