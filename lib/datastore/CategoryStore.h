/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef CATEGORYSTORE_H
#define CATEGORYSTORE_H

#include <Wt/WModelIndex.h>

class DBModel;

struct CategoryData {
	std::string title;
	int parent;
	int categories;
	int problems;
};

class CategoryStore {
public:
CategoryStore(DBModel *dbModel);
const std::map<int,CategoryData>& getCategories();
const CategoryData& getCategory(int id);
void addCategory(std::string title, const Wt::WModelIndex& parent, int row);

private:
DBModel *dbModel_;
std::map<int,CategoryData> categoryData_;

};

#endif // CATEGORYSTORE_H
