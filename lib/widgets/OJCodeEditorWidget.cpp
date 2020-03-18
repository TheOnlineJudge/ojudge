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
#include "OJCodeEditorWidget.h"

using namespace Wt;

OJCodeEditorWidget::OJCodeEditorWidget() : editorCodeSignal_(this,"editorcode") {
	create();
}

void OJCodeEditorWidget::render(WFlags<RenderFlag> flags) {

	if(flags.test(RenderFlag::Full)) {
		WStringStream strm;

		strm << "var self = " << jsRef() << ";";
		strm << "var editor = ace.edit(self);";
		strm << "editor.setTheme('ace/theme/textmate');";
		strm << "editor.session.setMode('ace/mode/c_cpp');";
		strm << "editor.setShowPrintMargin(false);";
		strm << "editor.session.on('change', function() { " << editorCodeSignal_.createCall({"editor.session.getValue()"}) << ";});";
		strm << "self.style.fontSize='16px';";

		doJavaScript(strm.str());
	}

	WCompositeWidget::render(flags);
}

void OJCodeEditorWidget::create() {

	setImplementation(std::unique_ptr<WWidget>(impl_ = new WContainerWidget()));
	setInline(false);

	WApplication *app = WApplication::instance();

	app->require("https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.8/ace.js","ace");

	editorCodeSignal_.connect(this,&OJCodeEditorWidget::getEditorCode);

}

const std::string& OJCodeEditorWidget::code() {

	return code_;
}

void OJCodeEditorWidget::setCode(const std::string& code) {

	code_ = code;

	WStringStream strm;

	strm << "setTimeout(function(){editor.session.setValue('" << code_ << "');},0)";

	doJavaScript(strm.str());
}

void OJCodeEditorWidget::loadCodeFromSession(const std::string& key) {

	WStringStream strm;

	std::cout << key << std::endl;

	strm << "if(sessionStorage.getItem('" << key << "')) {";
	strm << "setTimeout(function(){editor.session.setValue(atob(sessionStorage.getItem('" << key << "')));},0);";
	strm << "}";
	std::cout << strm.str() << std::endl;

	doJavaScript(strm.str());

}

void OJCodeEditorWidget::getEditorCode(std::string editorCode) {
	code_ = editorCode;
}
