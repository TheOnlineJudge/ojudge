/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef USERMODEL_H
#define USERMODEL_H

#include <Wt/WAbstractTableModel.h>

class UserStore;

class UserModel : public Wt::WAbstractTableModel {
public:
UserModel(UserStore *userStore);
virtual int columnCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual int rowCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual Wt::cpp17::any data(const Wt::WModelIndex& index, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;
virtual Wt::cpp17::any headerData(int section, Wt::Orientation orientation = Wt::Orientation::Horizontal, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;
virtual Wt::WModelIndex index(int row, int column, const Wt::WModelIndex& parent = Wt::WModelIndex()) const;

private:
UserStore *userStore_;
};

#endif // USERMODEL_H
