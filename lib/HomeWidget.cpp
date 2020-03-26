/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WText.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WAnimation.h>
#include "HomeWidget.h"

using namespace Wt;

HomeWidget::HomeWidget() {

	auto welcome = addWidget(cpp14::make_unique<WText>("<div>Welcome to the OJudge!</div>"));
	welcome->addStyleClass("oj-hometitle");

	auto intro = addWidget(cpp14::make_unique<WText>("<div>Here you will find hundreds of problems. They are like the ones used during programming contests, and are available in HTML and PDF formats. You can submit your sources in a variety of languages, trying to solve any of the problems available in our database.</div>"));

	auto boxes = addWidget(cpp14::make_unique<WContainerWidget>());
	boxes->addStyleClass("oj-boxes");
	boxes->addStyleClass("oj-maxwidth");

	auto boxesLayout = boxes->setLayout(cpp14::make_unique<WHBoxLayout>());
	boxesLayout->setContentsMargins(0,0,0,0);

	boxesLayout->addStretch(1);

	auto box1 = boxesLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0);
	box1->addStyleClass("oj-box oj-box1");
	box1->hide();

	boxesLayout->addStretch(1);

	auto box2 = boxesLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0);
	box2->addStyleClass("oj-box oj-box2");

	boxesLayout->addStretch(1);

	auto box3 = boxesLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0);
	box3->addStyleClass("oj-box oj-box3");

	boxesLayout->addStretch(1);

	WAnimation anim(AnimationEffect::Pop | AnimationEffect::Fade,TimingFunction::Linear,1000);

	box1->animateShow(anim);
	box2->animateShow(anim);
	box3->animateShow(anim);
}
