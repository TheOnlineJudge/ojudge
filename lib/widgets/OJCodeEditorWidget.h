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

class OJCodeEditorWidget : public Wt::WCompositeWidget {
public:
OJCodeEditorWidget();
const std::string& code();
void setCode(const std::string& code);
void loadCodeFromSession(const std::string& key);
void setLengthLimit(int limit);

private:
virtual void render(Wt::WFlags<Wt::RenderFlag> flags) override;
void create();
Wt::WContainerWidget *impl_;
std::string code_;
void getEditorCode(std::string editorCode);
void getAceThemes(std::string aceThemes);
Wt::WContainerWidget *editorWidget_;
std::unique_ptr<Wt::WContainerWidget> savedStatusTmp_;
Wt::WContainerWidget *savedStatus_;
std::unique_ptr<Wt::WText> cursorRowValueTmp_;
Wt::WText *cursorRowValue_;
std::unique_ptr<Wt::WText> cursorColumnValueTmp_;
Wt::WText *cursorColumnValue_;
Wt::WText *codeLengthValue_;
Wt::WText *codeLengthLimit_;
Wt::WProgressBar *codeLengthProgress_;
Wt::WComboBox *editorTheme_;
void editorFocus();
void ensureEditor();
bool isRendered_ = false;
int lengthLimit_ = 1024*40;
int tabSize_ = 4;
int fontSize_ = 16;
bool wordWrap_ = true;
Wt::JSignal<std::string> editorCodeSignal_;
Wt::JSignal<std::string> aceThemesSignal_;
Wt::Json::Array aceThemes_;
void themeChanged(int index);
};

#endif // OJCODEEDITORWIDGET_H
