// Shoot Them Up Game, All Rights Reserved

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD() {
    Super::DrawHUD();
    // DrawCrossHair();
}

void ASTUGameHUD::BeginPlay() {
    Super::BeginPlay();

    // 将UserWidget与对应游戏状态建立映射
    GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

    // 将UserWidget添加到场景中, 并设置为不可见
    for (auto GameWidgetPair : GameWidgets) {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;
        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    // 订阅OnMatchStateChanged委托
    if (GetWorld()) {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode) {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
        }
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

// 委托：游戏状态改变
void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State) {
    // 将当前UI设为不可见 
    if (CurrentWidget) CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    // 然后更改UI界面
    if (GameWidgets.Contains(State)) CurrentWidget = GameWidgets[State];
    if (CurrentWidget) CurrentWidget->SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogSTUGameHUD, Warning, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}
