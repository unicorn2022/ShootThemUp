// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUPauseWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public USTUBaseWidget {
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ClearPauseButton;

    // Initialize()时调用的函数
    virtual void NativeOnInitialized() override;

private:
    // 委托：点击按钮
    UFUNCTION()
    void OnClearPause();
};
