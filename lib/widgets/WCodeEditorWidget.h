/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef WCODEEDITORWIDGET_H
#define WCODEEDITORWIDGET_H

#include <Wt/WCompositeWidget.h>

namespace Wt {

class WCodeEditorWidget : public WCompositeWidget {
public:
	WCodeEditorWidget();
	const std::vector<unsigned char>& code();
	void setCode(const std::vector<unsigned char>& code);

private:
	virtual void render(WFlags<RenderFlag> flags) override;
	void create();
	WContainerWidget *impl_;
	std::vector<unsigned char> code_;
};

}

#endif // WCODEEDITORWIDGET_H
