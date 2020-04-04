/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WContainerWidget.h>
#include <Wt/WHBoxLayout.h>
#include "OJRatingSetWidget.h"

using namespace Wt;

OJRatingSetWidget::OJRatingSetWidget() {
	create();
}

void OJRatingSetWidget::render(WFlags<RenderFlag> flags) {

	if(flags.test(RenderFlag::Full)) {
		impl_->addStyleClass("oj-rating-set");
		auto layout = impl_->setLayout(cpp14::make_unique<WHBoxLayout>());
		layout->setContentsMargins(0,0,0,0);
		impl_->mouseWentOut().connect( [=] {
			setSelected(selectedStars_);
		});

		for(int i = 0; i < 5; i++) {
			stars_.push_back(layout->addWidget(cpp14::make_unique<WImage>("images/star_on.svg"),0));
		}

		for(auto s: stars_) {
			s->setWidth(40);
			s->mouseWentOver().connect( [=] {
				bool hit=false;
				for(auto cs: stars_) {
				        (hit ? cs->removeStyleClass("active") : cs->addStyleClass("active"));
				        if(cs==s) hit = true;
				}
			});
			s->clicked().connect( [=] {
				int ct = 1;
				for(auto cs: stars_) {
				        if(s==cs) setSelected(ct,true);
				        ct++;
				}
			});
		}
	}

	WCompositeWidget::render(flags);
}

void OJRatingSetWidget::create() {

	setImplementation(std::unique_ptr<WWidget>(impl_ = new WContainerWidget()));
	setInline(true);
}

void OJRatingSetWidget::setSelected(int stars, bool clicked) {

	selectedStars_ = (selectedStars_==stars && clicked ? 0 : stars);

	int ct = 0;
	for(auto cs: stars_) {
		(ct<selectedStars_ ? cs->addStyleClass("active") : cs->removeStyleClass("active"));
		ct++;
	}
}
