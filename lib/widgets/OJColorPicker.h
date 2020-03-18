/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef OJCOLORPICKER_H
#define OJCOLORPICKER_H

#include <Wt/WColor.h>
#include <Wt/WDllDefs.h>
#include <Wt/WFormWidget.h>
#include <Wt/WObject.h>
#include <Wt/WSignal.h>
#include <Wt/WWebWidget.h>

class OJColorPicker : public Wt::WFormWidget {
public:
OJColorPicker();
OJColorPicker(const Wt::WColor& color);
Wt::WColor color() const;
void setColor(const Wt::WColor& value);
Wt::EventSignal<>& colorInput();
virtual WT_USTRING valueText() const override;
virtual void setValueText(const WT_USTRING& value) override;

private:
Wt::WColor color_;
static constexpr const char* INPUT_SIGNAL = "input";

protected:
virtual Wt::DomElementType domElementType() const override;
virtual void setFormData(const Wt::WObject::FormData& formData) override;
};

#endif  // OJCOLORPICKER_H
