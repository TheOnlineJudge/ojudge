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
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WImage.h>
#include <Wt/WSpinBox.h>
#include <Wt/WToolBar.h>
#include <Wt/WPushButton.h>
#include <Wt/WComboBox.h>
#include "OJCodeEditorWidget.h"

using namespace Wt;

OJCodeEditorWidget::OJCodeEditorWidget() : editorCodeSignal_(this,"editorcode") {
	create();
}

void OJCodeEditorWidget::render(WFlags<RenderFlag> flags) {

	if(flags.test(RenderFlag::Full)) {

		auto mainLayout = impl_->setLayout(cpp14::make_unique<WVBoxLayout>());
		mainLayout->setContentsMargins(0,0,0,0);

		auto toolbarWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
		toolbarWidget->addStyleClass("oj-editor-toolbar");
		toolbarWidget->setMargin(0);

		auto toolbarLayout = toolbarWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
		toolbarLayout->setContentsMargins(0,0,0,0);

		auto fontSizeIconLayout = toolbarLayout->addLayout(cpp14::make_unique<WVBoxLayout>());
		fontSizeIconLayout->setContentsMargins(0,0,0,0);
		fontSizeIconLayout->addStretch(1);
		auto fontSizeIcon = fontSizeIconLayout->addWidget(cpp14::make_unique<WImage>("images/editor/font-size.svg"),0);
		fontSizeIcon->setHeight(18);
		fontSizeIcon->setToolTip("Select editor font size");
		fontSizeIconLayout->addStretch(1);
		auto fontSizeValue = toolbarLayout->addWidget(cpp14::make_unique<WSpinBox>());
		fontSizeValue->addStyleClass("form-inline");
		fontSizeValue->setValue(16);
		fontSizeValue->setWidth(75);
		fontSizeValue->setSuffix("px");
		fontSizeValue->setToolTip("Select editor font size");

		toolbarLayout->addSpacing(10);

		auto indentLessButton = toolbarLayout->addWidget(cpp14::make_unique<WPushButton>(),0);
		indentLessButton->setIcon(WLink("images/editor/indent-less.svg"));
		indentLessButton->setCanReceiveFocus(false);
		indentLessButton->setToolTip("Reduce indentation space");
		indentLessButton->clicked().connect(this,&OJCodeEditorWidget::editorFocus);

		auto indentMoreButton = toolbarLayout->addWidget(cpp14::make_unique<WPushButton>(),0);
		indentMoreButton->setIcon(WLink("images/editor/indent-more.svg"));
		indentMoreButton->setCanReceiveFocus(false);
		indentMoreButton->setToolTip("Increase indentation space");
		indentMoreButton->clicked().connect(this,&OJCodeEditorWidget::editorFocus);

		toolbarLayout->addSpacing(10);

		auto wrapButton = toolbarLayout->addWidget(cpp14::make_unique<WPushButton>(),0);
		wrapButton->setIcon(WLink("images/editor/wrap.svg"));
		wrapButton->setCheckable(true);
		wrapButton->setChecked(true);
		wrapButton->setCanReceiveFocus(false);
		wrapButton->setToolTip("Line wrapping");
		wrapButton->clicked().connect(this,&OJCodeEditorWidget::editorFocus);

		toolbarLayout->addSpacing(10);

		auto themeIconLayout = toolbarLayout->addLayout(cpp14::make_unique<WVBoxLayout>());
		themeIconLayout->setContentsMargins(0,0,0,0);
		themeIconLayout->addStretch(1);
		auto themeIcon = themeIconLayout->addWidget(cpp14::make_unique<WImage>("images/editor/theme.svg"),0);
		themeIcon->setHeight(18);
		themeIcon->setToolTip("Select editor theme");
		themeIconLayout->addStretch(1);
		auto editorTheme = toolbarLayout->addWidget(cpp14::make_unique<WComboBox>());
		editorTheme->setWidth(150);
		editorTheme->setToolTip("Select editor theme");

		toolbarLayout->addStretch(1);

		editorWidget_ = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);
		editorWidget_->setMargin(0);

		auto footerWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0);
		footerWidget->setMargin(0);
		footerWidget->addStyleClass("oj-editor-footer");
		
		auto footerLayout = footerWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
		footerLayout->setContentsMargins(0,0,0,0);

		auto codeLengthLabel = footerLayout->addWidget(cpp14::make_unique<WText>("Code length: "),0);
		auto codeLengthValue = footerLayout->addWidget(cpp14::make_unique<WText>("000"),0);
		
		footerLayout->addStretch(1);

		auto cursorRowLabel = footerLayout->addWidget(cpp14::make_unique<WText>("Row: "),0);
		auto cursorRowValue = footerLayout->addWidget(cpp14::make_unique<WText>("0"),0);
	
		footerLayout->addSpacing(10);

		auto cursorColumnLabel = footerLayout->addWidget(cpp14::make_unique<WText>("Column: "),0);
		auto cursorColumnValue = footerLayout->addWidget(cpp14::make_unique<WText>("0"),0);

		WStringStream strm;

		strm << "var self = " << editorWidget_->jsRef() << ";";
		strm << "var editor = ace.edit(self);";
		strm << "editor.setTheme('ace/theme/textmate');";
		strm << "editor.session.setMode('ace/mode/c_cpp');";
		strm << "editor.setShowPrintMargin(false);";
		strm << "editor.setFontSize(16);";
		strm << "editor.session.on('change', function() { " << editorCodeSignal_.createCall({"editor.session.getValue()"}) << ";});";

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

	strm << "if(sessionStorage.getItem('" << key << "')) {";
	strm << "setTimeout(function(){editor.session.setValue(atob(sessionStorage.getItem('" << key << "')));},0);";
	strm << "}";

	doJavaScript(strm.str());
}

void OJCodeEditorWidget::getEditorCode(std::string editorCode) {
	code_ = editorCode;
}

void OJCodeEditorWidget::editorFocus() {

	WStringStream strm;

	strm << "var editor = ace.edit(" << editorWidget_->jsRef() << ");";
	strm << "editor.focus();";

	doJavaScript(strm.str());

}
