/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef PROBLEMMODEL_H
#define PROBLEMMODEL_H

#include <Wt/WAbstractTableModel.h>

class ProblemStore;

class ProblemModel : public Wt::WAbstractTableModel {
public:

static constexpr Wt::ItemDataRole ProblemRowRole = Wt::ItemDataRole::User + 2;

ProblemModel(ProblemStore *problemStore);
virtual int columnCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual int rowCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual Wt::cpp17::any data(const Wt::WModelIndex& index, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;
virtual Wt::cpp17::any headerData(int section, Wt::Orientation orientation = Wt::Orientation::Horizontal, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;

void addProblem(long long id, std::string title, const Wt::WModelIndex& parent = Wt::WModelIndex());
void insertProblem(int row, const Wt::WModelIndex& parent);


private:
ProblemStore *problemStore_;
};

constexpr Wt::ItemDataRole ProblemModel::ProblemRowRole;

#endif // PROBLEMMODEL_H
