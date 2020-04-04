/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/WApplication.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WImage.h>
#include <Wt/WToolBar.h>
#include <Wt/WPushButton.h>
#include <Wt/WComboBox.h>
#include <Wt/Json/Parser.h>
#include <Wt/WStringListModel.h>
#include <Wt/Utils.h>
#include <Wt/WSpinBox.h>
#include "OJCodeEditorWidget.h"

using namespace Wt;

OJCodeEditorWidget::OJCodeEditorWidget() : editorCodeSignal_(this,"editorcode"), aceThemesSignal_(this,"acethemes") {
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
		fontSizeValue->setValue(fontSize_);
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
		wrapButton->setChecked(wordWrap_?true:false);
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
		editorTheme_ = toolbarLayout->addWidget(cpp14::make_unique<WComboBox>());
		editorTheme_->activated().connect(this,&OJCodeEditorWidget::themeChanged);
		editorTheme_->setWidth(150);
		editorTheme_->setToolTip("Select editor theme");

		toolbarLayout->addStretch(1);

		editorWidget_ = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),1);
		editorWidget_->setMargin(0);

		auto footerWidget = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0);
		footerWidget->setMargin(0);
		footerWidget->addStyleClass("oj-editor-footer");
		
		auto footerLayout = footerWidget->setLayout(cpp14::make_unique<WHBoxLayout>());
		footerLayout->setContentsMargins(0,0,0,0);

		footerLayout->addWidget(std::move(savedStatusTmp_),0);
		savedStatus_->setMargin(0);
		savedStatus_->addStyleClass("oj-editor-led");

		footerLayout->addSpacing(10);

		auto codeLengthLabel = footerLayout->addWidget(cpp14::make_unique<WText>("Code length: "),0);
		codeLengthProgress_ = footerLayout->addWidget(cpp14::make_unique<OJCodeEditorProgress>(),0);
		codeLengthProgress_->setMaximum(lengthLimit_);
		auto codeLengthBytes = footerLayout->addWidget(cpp14::make_unique<WText>(" bytes"),0);
		
		footerLayout->addStretch(1);

		auto cursorRowLabel = footerLayout->addWidget(cpp14::make_unique<WText>("Row: "),0);
		cursorRowValueTmp_->addStyleClass("oj-editor-cursorinfo");
		footerLayout->addWidget(std::move(cursorRowValueTmp_),0);
	
		footerLayout->addSpacing(10);

		auto cursorColumnLabel = footerLayout->addWidget(cpp14::make_unique<WText>("Column: "),0);
		cursorColumnValueTmp_->addStyleClass("oj-editor-cursorinfo");
		footerLayout->addWidget(std::move(cursorColumnValueTmp_),0);

		WStringStream strm;

		strm << "var self = " << editorWidget_->jsRef() << ";";
		strm << "var editor = ace.edit(self);";
		strm << "var timeout = null;";
		strm << "editor.setTheme('ace/theme/" << theme_ << "');";
		strm << "editor.session.setMode('ace/mode/c_cpp');";
		strm << "editor.session.setUseSoftTabs(false);";
		strm << "editor.session.setTabSize(" << tabSize_ << ");";
		strm << "editor.session.setUseWrapMode(" << (wordWrap_?"true":"false") << ");";
		strm << "editor.setShowPrintMargin(false);";
		strm << "editor.setFontSize(" << fontSize_ << ");";
		strm << "ace.config.loadModule('ace/ext/themelist',function(themelist) {"
			<< aceThemesSignal_.createCall({"JSON.stringify(themelist)"}) << ";});";
		strm << "editor.session.on('change', function() {"
			<< "clearTimeout(timeout);"
			<< "timeout = setTimeout(function() { "
			<< editorCodeSignal_.createCall({"encodeURIComponent(editor.session.getValue())"}) << ";},1000);});";
		strm << "editor.session.on('change', function() { " << savedStatus_->jsRef() << ".classList.add('oj-unsaved');});";
		strm << "editor.selection.on('changeCursor',function() { var cP = editor.getCursorPosition();"
			<< cursorRowValue_->jsRef() << ".innerText = cP.row;"
			<< cursorColumnValue_->jsRef() << ".innerText = cP.column;});";

		doJavaScript(strm.str());

		indentLessButton->clicked().connect([=] { 
			ensureEditor();
			tabSize_ = (tabSize_>1?--tabSize_:1);
			doJavaScript("editor.session.setTabSize(" + std::to_string(tabSize_) + ");");
			saveSettings();
		});

		indentMoreButton->clicked().connect([=] {
			ensureEditor();
			tabSize_++;
			doJavaScript("editor.session.setTabSize(" + std::to_string(tabSize_) + ");");
			saveSettings();
		});

		wrapButton->clicked().connect([=] {
			ensureEditor();
			wordWrap_ = wrapButton->isChecked();
			doJavaScript("editor.session.setUseWrapMode(" + std::string(wordWrap_?"true":"false") + ");");
			saveSettings();
		});

		fontSizeValue->valueChanged().connect([=] (int fontSize) {
			ensureEditor();
			fontSize_ = fontSize;
			doJavaScript("editor.setFontSize(" + std::to_string(fontSize_) + ");");
			saveSettings();
		});
	}

	WCompositeWidget::render(flags);

	isRendered_ = true;
	editorFocus();
}

void OJCodeEditorWidget::create() {

	setImplementation(std::unique_ptr<WWidget>(impl_ = new WContainerWidget()));
	setInline(false);

	WApplication *app = WApplication::instance();

	app->require("https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.8/ace.js","ace");

	savedStatusTmp_ = cpp14::make_unique<WContainerWidget>();
	savedStatus_ = savedStatusTmp_.get();

	cursorRowValueTmp_ = cpp14::make_unique<WText>("0");
	cursorRowValue_ = cursorRowValueTmp_.get();

	cursorColumnValueTmp_ = cpp14::make_unique<WText>("0");
	cursorColumnValue_ = cursorColumnValueTmp_.get();

	editorCodeSignal_.connect(this,&OJCodeEditorWidget::getEditorCode);
	aceThemesSignal_.connect(this,&OJCodeEditorWidget::getAceThemes);

}

void OJCodeEditorWidget::ensureEditor() {

	if(!isRendered_) return;

	WStringStream strm;

	strm << "if(!editor) {";
	strm << "var editor = ace.edit(" << editorWidget_->jsRef() << ");";
	strm << "}";

	doJavaScript(strm.str());
}

std::string OJCodeEditorWidget::code() {

	return Utils::urlDecode(code_);
}

void OJCodeEditorWidget::setCode(const std::string& code) {

	ensureEditor();

	code_ = code;

	WStringStream strm;
	strm << "setTimeout(function(){";
	strm << "editor.session.setValue('" << code_ << "');";
	strm << savedStatus_->jsRef() << ".classList.remove('oj-unsaved');";
	strm << "},0)";
	doJavaScript(strm.str());
}

void OJCodeEditorWidget::loadCodeFromSession(const std::string& key) {

	ensureEditor();

	WStringStream strm;
	strm << "if(sessionStorage.getItem('" << key << "')) {";
	strm << "setTimeout(function(){";
	strm << "editor.session.setValue(decodeURIComponent(atob(sessionStorage.getItem('" << key << "'))));";
	strm << savedStatus_->jsRef() << ".classList.remove('oj-unsaved');";
	strm << "},0);";
	strm << "}";
	doJavaScript(strm.str());
}

void OJCodeEditorWidget::setSettings(OJCodeEditorSettings& settings) {

	fontSize_ = settings.fontsize;
	tabSize_ = settings.indent;
	wordWrap_ = settings.wrap;
	theme_ = settings.theme;
}

void OJCodeEditorWidget::getEditorCode(std::string editorCode) {
	code_ = editorCode;

	codeLengthProgress_->setValue(Utils::urlDecode(code_).size());	

	savedStatus_->removeStyleClass("oj-unsaved",true);
}

void OJCodeEditorWidget::getAceThemes(std::string aceThemes) {

	Json::Object result;
	Json::parse(aceThemes,result);
	aceThemes_ = result.get("themes");

	WStringListModel *comboModel = (WStringListModel *)editorTheme_->model().get();
	int ct = 0;
	for(const Json::Object &i: aceThemes_) {
		comboModel->addString(std::string(i.get("caption")) + (i.get("isDark")?" (dark)":""));
		comboModel->setData(comboModel->index(ct),std::string(i.get("name")),ItemDataRole::User + 1);
		comboModel->setData(comboModel->index(ct),std::string(i.get("theme")),ItemDataRole::User + 2);
		ct++;
	}

	editorTheme_->setCurrentIndex(comboModel->match(comboModel->index(0),ItemDataRole::User + 1,theme_,1,MatchFlag::Exactly | MatchFlag::Wrap).at(0).row());
}

void OJCodeEditorWidget::themeChanged(int index) {

	WStringListModel *comboModel = (WStringListModel *)editorTheme_->model().get();

	ensureEditor();

	WStringStream strm;
	strm << "editor.setTheme('"
		<< cpp17::any_cast<std::string>(comboModel->data(comboModel->index(index),ItemDataRole::User + 2))
		<< "');";
	strm << "editor.focus();";

	doJavaScript(strm.str());

	theme_ = cpp17::any_cast<std::string>(comboModel->data(comboModel->index(index),ItemDataRole::User + 1));
	saveSettings();
}

void OJCodeEditorWidget::editorFocus() {

	ensureEditor();

	WStringStream strm;
	strm << "editor.focus();";
	doJavaScript(strm.str());
}

void OJCodeEditorWidget::setLengthLimit(int limit) {

	lengthLimit_ = limit;
	codeLengthProgress_->setMaximum(lengthLimit_);
}

void OJCodeEditorWidget::saveSettings() {

	OJCodeEditorSettings settings;

	settings.fontsize = fontSize_;
	settings.indent = tabSize_;
	settings.wrap = wordWrap_;
	settings.theme = theme_;

	settingsChanged_.emit(settings);
}

OJCodeEditorWidget::OJCodeEditorProgress::OJCodeEditorProgress() {

}

WString OJCodeEditorWidget::OJCodeEditorProgress::text() const {
	return WString(std::to_string((int)value()) + " / " + std::to_string((int)maximum()));
}

void OJCodeEditorWidget::OJCodeEditorProgress::updateBar(DomElement& bar) {
	WProgressBar::updateBar(bar);
}
