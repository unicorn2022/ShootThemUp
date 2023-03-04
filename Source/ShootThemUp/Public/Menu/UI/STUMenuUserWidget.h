// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUMenuUserWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUMenuUserWidget : public UUserWidget {
    GENERATED_BODY()

protected:
    // 开始游戏按钮
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    // 退出游戏按钮
    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();
};
