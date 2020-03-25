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
class CategoryModel;

class ProblemModel : public Wt::WAbstractTableModel {
public:

static constexpr Wt::ItemDataRole ProblemRowRole = Wt::ItemDataRole::User + 2;

ProblemModel(ProblemStore *problemStore, std::shared_ptr<CategoryModel> categoryModel);
virtual int columnCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual int rowCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual Wt::cpp17::any data(const Wt::WModelIndex& index, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;
virtual Wt::cpp17::any headerData(int section, Wt::Orientation orientation = Wt::Orientation::Horizontal, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;
virtual Wt::WModelIndex index(int row, int column, const Wt::WModelIndex& parent = Wt::WModelIndex()) const;

void addProblem(long long id, std::string title, const Wt::WModelIndex& parent = Wt::WModelIndex());
void setCategories(long long id, const Wt::WModelIndexSet& categories);
const Wt::WModelIndexSet getCategories(long long id);
void insertProblem(int row, const Wt::WModelIndex& parent);


private:
ProblemStore *problemStore_;
std::shared_ptr<CategoryModel> categoryModel_;
};

constexpr Wt::ItemDataRole ProblemModel::ProblemRowRole;

#endif // PROBLEMMODEL_H
