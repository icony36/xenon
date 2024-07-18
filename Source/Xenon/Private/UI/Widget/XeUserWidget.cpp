// Copyright CoNY. All rights reserved.


#include "UI/Widget/XeUserWidget.h"

void UXeUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetContorllerSet();
}
