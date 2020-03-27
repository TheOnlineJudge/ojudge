/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef OJRATINGVIEWWIDGET_H
#define OJRATINGVIEWWIDGET_H

#include <Wt/WCompositeWidget.h>

class OJRatingViewWidget : public Wt::WCompositeWidget {
public:
OJRatingViewWidget();
void setRating(int percent);

private:
virtual void render(Wt::WFlags<Wt::RenderFlag> flags) override;
void create();
Wt::WContainerWidget *impl_;
Wt::WContainerWidget *background_;
Wt::WContainerWidget *foreground_;
};

#endif // OJRATINGVIEWWIDGET_H
