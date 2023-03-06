// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUMenuUserWidget.generated.h"

class UButton;
class UHorizontalBox;
class USTUGameInstance;
class USTULevelItemWidget;

UCLASS()
class SHOOTTHEMUP_API USTUMenuUserWidget : public USTUBaseWidget {
    GENERATED_BODY()

protected:
    // 开始游戏按钮
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    // 退出游戏按钮
    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    // 水平框：关卡选择
    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* LevelItemBox;

    // Widget：单个关卡
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    // 动画：隐藏整个UI界面, 用于关卡加载时防止冻结
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* HideAnimation;

    virtual void NativeOnInitialized() override;
    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
    // 单个关卡选择控件
    UPROPERTY()
    TArray<USTULevelItemWidget*> LevelItemWidgets;

    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();

    // 初始化关卡选择控件
    void InitLevelItems();
    // 委托：当前关卡被选中
    void OnLevelSelected(const FLevelData& Data);
    USTUGameInstance* GetSTUGameInstance() const;
};
