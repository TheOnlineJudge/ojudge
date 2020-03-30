/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "../datastore/CountryStore.h"

#include "CountryModel.h"

using namespace Wt;

CountryModel::CountryModel(CountryStore *countryStore) : WAbstractTableModel(), countryStore_(countryStore) {

}

int CountryModel::columnCount(const WModelIndex& parent) const {
	return 1;
}

int CountryModel::rowCount(const WModelIndex& parent) const {
	return countryStore_->countryCodes().size();
}

cpp17::any CountryModel::data(const WModelIndex& index, ItemDataRole role) const {

	if(role == ItemDataRole::Display) {
		switch(index.column()) {
		case 0:
			return countryStore_->countryName(countryStore_->getCountry(index.row()));
		}
	} else if(role == ItemDataRole::Decoration) {
                if(index.column()==0) {
                        return countryStore_->countryFlag(countryStore_->getCountry(index.row()))->url();
                }
        } else if(role == ItemDataRole::StyleClass) {
		if(index.column()==0) {
			return std::string("oj-country-table-item");
		}
	}

	return cpp17::any();

}

cpp17::any CountryModel::headerData(int section, Orientation orientation, ItemDataRole role) const {
	if(orientation == Orientation::Horizontal && role == ItemDataRole::Display) {
		switch(section) {
		case 0:
			return std::string("Country");
		default:
			return cpp17::any();
		}
	}

	return cpp17::any();
}
