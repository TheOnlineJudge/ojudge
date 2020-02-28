/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef ADMINCATEGORYWIDGET_H
#define ADMINCATEGORYWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WTreeView.h>
#include <Wt/WDialog.h>
#include <Wt/WLineEdit.h>
#include <Wt/WCheckBox.h>
#include <Wt/WWidget.h>
#include "viewmodels/CategoryModel.h"

#include <Wt/WAbstractItemDelegate.h>

class AdminCategoryWidget : public Wt::WContainerWidget {
public:
AdminCategoryWidget(const std::shared_ptr<CategoryModel> catmodel);

private:
const std::shared_ptr<CategoryModel> catmodel_;
Wt::WVBoxLayout *mainLayout_;
Wt::WTreeView *treeWidget_;
Wt::WDialog *addEditDialog_;
Wt::WCheckBox *rootCheckBox_;
Wt::WTreeView *selectParent_;
Wt::WLineEdit *title_;
void showAddEditDialog(const Wt::WModelIndex& index = Wt::WModelIndex());
void addEditDialogDone(Wt::DialogCode code);

class AdminActionsDelegate : public Wt::WAbstractItemDelegate {
public:
AdminActionsDelegate();
virtual std::unique_ptr<Wt::WWidget> update(Wt::WWidget *widget, const Wt::WModelIndex& index, Wt::WFlags<Wt::ViewItemRenderFlag> flags) override;
Wt::Signal<const Wt::WModelIndex&>& editCategory() { return editCategory_; }
Wt::Signal<const Wt::WModelIndex&>& deleteCategory() { return deleteCategory_; }
Wt::Signal<const Wt::WModelIndex&,bool>& moveCategory() { return moveCategory_; }

private:
struct WidgetRef {
	std::unique_ptr<Wt::WWidget> created;
	Wt::WWidget *w;
	WidgetRef(Wt::WWidget *widget) : w(widget) { }
};

Wt::Signal<const Wt::WModelIndex&> editCategory_ ;
Wt::Signal<const Wt::WModelIndex&> deleteCategory_ ;
Wt::Signal<const Wt::WModelIndex&,bool> moveCategory_ ;

};

};

#endif // ADMINCATEGORYWIDGET_H
