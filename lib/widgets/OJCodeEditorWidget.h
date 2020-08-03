/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef OJCODEEDITORWIDGET_H
#define OJCODEEDITORWIDGET_H

#include <Wt/WCompositeWidget.h>
#include <Wt/WJavaScript.h>
#include <Wt/WPushButton.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WProgressBar.h>
#include <Wt/WComboBox.h>
#include <Wt/Json/Object.h>
#include <Wt/Json/Array.h>
#include <map>

struct OJCodeEditorSettings {
	int fontsize;
	int indent;
	bool wrap;
	std::string theme;
	std::string style;
};

class OJCodeEditorWidget : public Wt::WCompositeWidget {
public:
OJCodeEditorWidget();
OJCodeEditorWidget(std::map<std::string, std::string> languages);
std::string code();
void setCode(const std::string& code);
void loadCodeFromSession(const std::string& key);
void setLengthLimit(int limit);
void setSettings(OJCodeEditorSettings& settings);
Wt::Signal<OJCodeEditorSettings&>& settingsChanged() {
	return settingsChanged_;
}

private:
class OJCodeEditorProgress : public Wt::WProgressBar {
public:
OJCodeEditorProgress();
virtual Wt::WString text() const;

private:
virtual void updateBar(Wt::DomElement& bar);
};

virtual void render(Wt::WFlags<Wt::RenderFlag> flags) override;
void create();
Wt::WContainerWidget *impl_;
std::string code_;
void getEditorCode(std::string editorCode);
void getAceThemes(std::string aceThemes);
void getAceStyles();
Wt::WContainerWidget *editorWidget_;
std::unique_ptr<Wt::WContainerWidget> savedStatusTmp_;
Wt::WContainerWidget *savedStatus_;
std::unique_ptr<Wt::WText> cursorRowValueTmp_;
Wt::WText *cursorRowValue_;
std::unique_ptr<Wt::WText> cursorColumnValueTmp_;
Wt::WText *cursorColumnValue_;
OJCodeEditorProgress *codeLengthProgress_;
Wt::WComboBox *editorTheme_;
Wt::WComboBox *editorStyle_;
void editorFocus();
void ensureEditor();
void saveSettings();
bool isRendered_ = false;
int lengthLimit_ = 1024*40;
int tabSize_ = 4;
int fontSize_ = 16;
bool wordWrap_ = true;
std::string theme_ = "textmate";
std::string style_ = "c_cpp";
Wt::JSignal<std::string> editorCodeSignal_;
Wt::JSignal<std::string> aceThemesSignal_;
std::map<std::string,std::string> languages_;
Wt::Json::Array aceThemes_;
void themeChanged(int index);
void styleChanged(int index);
Wt::Signal<OJCodeEditorSettings&> settingsChanged_;
};

#endif // OJCODEEDITORWIDGET_H
