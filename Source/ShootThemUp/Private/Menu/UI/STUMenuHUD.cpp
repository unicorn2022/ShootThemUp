// Shoot Them Up Game, All Rights Reserved

#include "Menu/UI/STUMenuHUD.h"
#include "UI/STUBaseWidget.h"

void ASTUMenuHUD::BeginPlay() {
    Super::BeginPlay();

    // 创建菜单控件, 并添加到视图
    if (MenuWidgetClass) {
        const auto MenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidget) {
            MenuWidget->AddToViewport();
            MenuWidget->Show();
        }
    }
}
