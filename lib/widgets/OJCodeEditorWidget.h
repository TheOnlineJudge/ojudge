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

class OJCodeEditorWidget : public Wt::WCompositeWidget {
public:
OJCodeEditorWidget();
const std::string& code();
void setCode(const std::string& code);
void loadCodeFromSession(const std::string& key);

private:
virtual void render(Wt::WFlags<Wt::RenderFlag> flags) override;
void create();
Wt::WContainerWidget *impl_;
std::string code_;
void getEditorCode(std::string editorCode);
Wt::JSignal<std::string> editorCodeSignal_;
};

#endif // OJCODEEDITORWIDGET_H
