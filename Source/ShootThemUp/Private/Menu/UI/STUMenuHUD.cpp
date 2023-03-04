// Shoot Them Up Game, All Rights Reserved

#include "Menu/UI/STUMenuHUD.h"
#include "Blueprint/UserWidget.h"

void ASTUMenuHUD::BeginPlay() {
    Super::BeginPlay();

    // 创建菜单控件, 并添加到视图
    if (MenuWidgetClass) {
        const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidget) MenuWidget->AddToViewport();
    }
}
