/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include "OJProblemViewerWidget.h"

using namespace Wt;

OJProblemViewerWidget::OJProblemViewerWidget() {
	create();
}

void OJProblemViewerWidget::render(WFlags<RenderFlag> flags) {

	if(flags.test(RenderFlag::Full)) {
		//
	}

	WCompositeWidget::render(flags);
}

void OJProblemViewerWidget::create() {

	setImplementation(std::unique_ptr<WWidget>(impl_ = new WContainerWidget()));
	setInline(false);

	WApplication *app = WApplication::instance();

	app->require("https://polyfill.io/v3/polyfill.min.js?features=es6","polyfill");
	app->require("https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-chtml.js","mathjax");

	contentWidget_ = impl_->addWidget(cpp14::make_unique<WText>());
	contentWidget_->setTextFormat(TextFormat::UnsafeXHTML);
	contentWidget_->addStyleClass("oj-problem-viewer");
}

void OJProblemViewerWidget::setContent(const std::string& content) {

	contentWidget_->setText(WString(content));
	doJavaScript("setTimeout(function(){MathJax.typeset();},0)");
}
