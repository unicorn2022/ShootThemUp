// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUGameHUD.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD {
    GENERATED_BODY()
public:
    virtual void DrawHUD() override;

protected:
    // 游戏过程中的UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

    // 游戏暂停时的UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    virtual void BeginPlay() override;

private:
    // 将游戏状态与对应UI建立映射关系
    UPROPERTY()
    TMap<ESTUMatchState, UUserWidget*> GameWidgets;

    // 游戏当前UI
    UPROPERTY()
    UUserWidget* CurrentWidget = nullptr;

private:
    // 绘制屏幕中心的十字准线
    void DrawCrossHair();
    
    // 委托：游戏状态改变
    void OnMatchStateChanged(ESTUMatchState State);
};
