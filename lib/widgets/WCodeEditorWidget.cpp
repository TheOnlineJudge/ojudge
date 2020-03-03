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
#include "WCodeEditorWidget.h"

namespace Wt {

WCodeEditorWidget::WCodeEditorWidget() {
	create();
}

void WCodeEditorWidget::render(WFlags<RenderFlag> flags) {

	if(flags.test(RenderFlag::Full)) {
		WApplication *app = WApplication::instance();

		WStringStream strm;

		strm << "{";
		strm << "var self = " << jsRef() << ";";
		strm << "var editor = ace.edit(self);";
		strm << "editor.setTheme('ace/theme/monokai');";
		strm << "editor.session.setMode('ace/mode/javascript');";
		setJavaScriptMember(WT_RESIZE_JS,
			"function(self, w, h) {"
                          """if (w >= 0) "
                          ""  "self.style.width=w + 'px';"
                          """if (h >= 0) "
                          ""  "self.style.height=h + 'px';"
                          """if (self.editor)"
                          """ self.editor.resize();"
                          "}");
		strm << "}";

		doJavaScript(strm.str());
	}

	WCompositeWidget::render(flags);
}

void WCodeEditorWidget::create() {

	setImplementation(std::unique_ptr<WWidget>(impl_ = new WContainerWidget()));
	setInline(false);

	WApplication *app = WApplication::instance();

	app->require("https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.8/ace.js","ace");

}

const std::vector<unsigned char>& WCodeEditorWidget::code() {

	return code_;

}

void WCodeEditorWidget::setCode(const std::vector<unsigned char>& code) {

}

}
