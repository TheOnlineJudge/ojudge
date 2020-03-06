/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef OJCODEEDITORWIDGET_H
#define OJCODEEDITORWIDGET_H

#include <Wt/WCompositeWidget.h>

class OJCodeEditorWidget : public Wt::WCompositeWidget {
public:
	OJCodeEditorWidget();
	const std::vector<unsigned char>& code();
	void setCode(const std::vector<unsigned char>& code);

private:
	virtual void render(Wt::WFlags<Wt::RenderFlag> flags) override;
	void create();
	Wt::WContainerWidget *impl_;
	std::vector<unsigned char> code_;
};

#endif // OJCODEEDITORWIDGET_H
