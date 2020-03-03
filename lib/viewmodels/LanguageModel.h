/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include <Wt/WAbstractTableModel.h>

class LanguageStore;

class LanguageModel : public Wt::WAbstractTableModel {
public:

static constexpr Wt::ItemDataRole LanguageRowRole = Wt::ItemDataRole::User + 3;

LanguageModel(LanguageStore *languageStore);
virtual int columnCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual int rowCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual Wt::cpp17::any data(const Wt::WModelIndex& index, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;
virtual Wt::cpp17::any headerData(int section, Wt::Orientation orientation = Wt::Orientation::Horizontal, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;

/*void addProblem(long long id, std::string title, const Wt::WModelIndex& parent = Wt::WModelIndex());
void insertProblem(int row, const Wt::WModelIndex& parent);*/

private:
LanguageStore *languageStore_;
};

constexpr Wt::ItemDataRole LanguageModel::LanguageRowRole;

#endif // LANGUAGEMODEL_H
