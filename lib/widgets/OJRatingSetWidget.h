/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef OJRATINGSETWIDGET_H
#define OJRATINGSETWIDGET_H

#include <Wt/WCompositeWidget.h>
#include <Wt/WText.h>
#include <Wt/WEvent.h>
#include <Wt/WImage.h>

class OJRatingSetWidget : public Wt::WCompositeWidget {
public:
OJRatingSetWidget();
void setSelected(int stars, bool clicked = false);

private:
virtual void render(Wt::WFlags<Wt::RenderFlag> flags) override;
void create();
Wt::WContainerWidget *impl_;
std::vector<Wt::WImage*> stars_;
int selectedStars_ = 0;
};

#endif // OJRATINGSETWIDGET_H
