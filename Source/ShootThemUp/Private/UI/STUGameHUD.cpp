// Shoot Them Up Game, All Rights Reserved


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD() {
    Super::DrawHUD();
    // DrawCrossHair();
}

void ASTUGameHUD::BeginPlay() {
    Super::BeginPlay();
    UE_LOG(LogSTUGameHUD, Warning, TEXT("PlayerHUDWidgetClass != nullptr : %d"), PlayerHUDWidgetClass != nullptr);
    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (PlayerHUDWidget) {
        PlayerHUDWidget->AddToViewport();
    }
}

// 绘制屏幕中心的十字准线
void ASTUGameHUD::DrawCrossHair() {
    const float CenterX = Canvas->SizeX * 0.5f, CenterY = Canvas->SizeY * 0.5f;

    
    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    // 水平线
    DrawLine(CenterX - HalfLineSize, CenterY, CenterX + HalfLineSize, CenterY, LineColor, LineThickness);
    // 垂直线
    DrawLine(CenterX, CenterY - HalfLineSize, CenterX, CenterY + HalfLineSize, LineColor, LineThickness);
}
