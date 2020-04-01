/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef COUNTRYMODEL_H
#define COUNTRYMODEL_H

#include <Wt/WAbstractTableModel.h>

class CountryStore;

class CountryModel : public Wt::WAbstractTableModel {
public:

static constexpr Wt::ItemDataRole CountryCodeRole = Wt::ItemDataRole::User + 1;

CountryModel(CountryStore *countryStore);
virtual int columnCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual int rowCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual Wt::cpp17::any data(const Wt::WModelIndex& index, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;
virtual Wt::cpp17::any headerData(int section, Wt::Orientation orientation = Wt::Orientation::Horizontal, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;

private:
CountryStore *countryStore_;
};

constexpr Wt::ItemDataRole CountryModel::CountryCodeRole;

#endif // COUNTRYMODEL_H
