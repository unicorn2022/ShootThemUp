// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance {
    GENERATED_BODY()

public:
    FName GetStartupLevelName() const { return StartupLevelName; }
    FName GetMenuLevelName() const { return MenuLevelName; }

protected:
    // 开始关卡的名称
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName StartupLevelName = NAME_None;

    // 主菜单关卡的名称
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MenuLevelName = NAME_None;
};
