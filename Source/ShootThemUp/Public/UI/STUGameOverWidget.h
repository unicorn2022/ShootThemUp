// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public USTUBaseWidget {
    GENERATED_BODY()

protected:
    // 垂直框：显示角色信息
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStateBox;

    // Widget：角色信息单行显示UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStateRowWidgetClass;

    // 按钮：重新开始游戏
    UPROPERTY(meta = (BindWidget))
    UButton* ResetLevelButton;

    // Initialize()时调用的函数
    virtual void NativeOnInitialized() override;

private:
    void OnMatchStateChanged(ESTUMatchState State);
    // 更新角色信息
    void UpdatePlayerState();

    // 委托：重新开始游戏
    UFUNCTION()
    void OnResetLevel();
};
