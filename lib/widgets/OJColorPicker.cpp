#include <Wt/WColor.h>
#include <Wt/WDllDefs.h>
#include <Wt/WFormWidget.h>
#include <Wt/WObject.h>
#include <Wt/WSignal.h>
#include <Wt/WString.h>
#include <Wt/WWebWidget.h>

#include <string>

#include "OJColorPicker.h"

using namespace Wt;

OJColorPicker::OJColorPicker() {
	setInline(true);
	setFormObject(true);
	setAttributeValue("type", "color");
}

OJColorPicker::OJColorPicker(const WColor& color) {
	setInline(true);
	setFormObject(true);
	setAttributeValue("type", "color");
	setColor(color);
}

WColor OJColorPicker::color() const {
	return color_;
}

void OJColorPicker::setColor(const WColor& value) {
	color_ = value;
	doJavaScript(jsRef() + ".value = " + WWebWidget::jsStringLiteral(value.cssText()) + ";");
}

EventSignal<>& OJColorPicker::colorInput() {
	return *voidEventSignal(INPUT_SIGNAL, true);
}

DomElementType OJColorPicker::domElementType() const {
	return DomElementType::INPUT;
}

void OJColorPicker::setFormData(const FormData& formData) {
	if (isReadOnly())
		return;

	const std::string& value = formData.values[0];
	color_ = WColor(value);
}

WT_USTRING OJColorPicker::valueText() const {
	return color().cssText();
}

void OJColorPicker::setValueText(const WT_USTRING& value) {
	setColor(WColor(value));
}
