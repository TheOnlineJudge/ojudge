/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef OJPROBLEMVIEWERWIDGET_H
#define OJPROBLEMVIEWERWIDGET_H

#include <Wt/WCompositeWidget.h>
#include <Wt/WText.h>

class OJProblemViewerWidget : public Wt::WCompositeWidget {
public:
	OJProblemViewerWidget();
	void setContent(const std::string& content);

private:
	virtual void render(Wt::WFlags<Wt::RenderFlag> flags) override;
	void create();
	Wt::WContainerWidget *impl_;
	Wt::WText *contentWidget_;
};

#endif // OJPROBLEMVIEWERWIDGET_H
