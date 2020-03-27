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
#include <Wt/WImage.h>
#include "OJRatingViewWidget.h"

using namespace Wt;

OJRatingViewWidget::OJRatingViewWidget() {
	create();
}

void OJRatingViewWidget::render(WFlags<RenderFlag> flags) {

	if(flags.test(RenderFlag::Full)) {
		impl_->addStyleClass("oj-rating-view");

		background_->setMargin(0);
		background_->addStyleClass("oj-rating-view-background");
		auto backLayout = background_->setLayout(cpp14::make_unique<WHBoxLayout>());
		backLayout->setContentsMargins(0,0,0,0);

		foreground_->setMargin(0);
		foreground_->addStyleClass("oj-rating-view-foreground");
		auto foreLayout = foreground_->setLayout(cpp14::make_unique<WHBoxLayout>());
		foreLayout->setContentsMargins(0,0,0,0);

		for(int i = 0; i < 5; i++) {
			auto tmpWidget = backLayout->addWidget(cpp14::make_unique<WImage>("images/star_off.svg"),0);
			tmpWidget->setWidth(24);
		}

		for(int i = 0; i < 5; i++) {
			auto tmpWidget = foreLayout->addWidget(cpp14::make_unique<WImage>("images/star_on.svg"),0);
			tmpWidget->setWidth(24);
		}
	}

	WCompositeWidget::render(flags);
}

void OJRatingViewWidget::create() {

	setImplementation(std::unique_ptr<WWidget>(impl_ = new WContainerWidget()));
	setInline(true);

	background_ = impl_->addWidget(cpp14::make_unique<WContainerWidget>());
	foreground_ = impl_->addWidget(cpp14::make_unique<WContainerWidget>());
}

void OJRatingViewWidget::setRating(int percent) {
	foreground_->setWidth(WLength(percent,LengthUnit::Percentage));
}
