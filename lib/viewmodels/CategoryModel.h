/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include <Wt/WAbstractItemModel.h>

class CategoryStore;

class CategoryModel : public Wt::WAbstractItemModel {
public:

static constexpr Wt::ItemDataRole CategoryIdRole = Wt::ItemDataRole::User + 1;

CategoryModel(CategoryStore *categoryStore_);
virtual int columnCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual int rowCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual Wt::WModelIndex parent(const Wt::WModelIndex& index) const override;
virtual Wt::WModelIndex index(int row, int column, const Wt::WModelIndex& parent = Wt::WModelIndex()) const override;
virtual Wt::cpp17::any data(const Wt::WModelIndex& index, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;
virtual Wt::cpp17::any headerData(int section, Wt::Orientation orientation = Wt::Orientation::Horizontal, Wt::ItemDataRole role = Wt::ItemDataRole::Display) const override;
void insertCategory(int row, const Wt::WModelIndex& parent, int categoryId);

void addCategory(std::string title, Wt::WModelIndex& parent) ;

private:
CategoryStore *categoryStore_;

struct ChildIndex {
	int parentId;
	int index;

	ChildIndex(int aParentId, int aIndex) : parentId(aParentId), index(aIndex) {
	}

	bool operator< (const ChildIndex& other) const {
		if(parentId < other.parentId) return true;
		else if(parentId > other.parentId) return false;
		else return index < other.index;
	}

	bool equals(Wt::cpp17::any o) {
		ChildIndex *other = Wt::cpp17::any_cast<ChildIndex *>(o);
		return parentId == other->parentId && index == other->index;
	}

	int hashCode() {
		int hash = 1;
		hash = hash * 31 + parentId;
		hash = hash * 31 + index;
		return hash;
	}
};

std::map<int,int> childIndex_;
std::map<int,int> dbToInternal_;

class Tree {
public:
Tree(int parentId, int index, int categoryId, int rowCount);

int parentId() const;
int index() const;
int categoryId() const;
int rowCount() const;
void addRow();

private:
ChildIndex index_;
int categoryId_;
int rowCount_;
};

mutable std::vector<Tree> treeData_;

typedef std::map<ChildIndex, int> ChildPointerMap;
mutable ChildPointerMap childPointer_;

int getTreeId(int parentId, int childIndex, int categoryId=0, int rowCount=0) const;
};

constexpr Wt::ItemDataRole CategoryModel::CategoryIdRole;

#endif // CATEGORYMODEL_H
