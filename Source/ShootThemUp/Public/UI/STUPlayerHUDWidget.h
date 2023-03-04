// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget {
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    // 玩家是否存活
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;

    // 玩家是否处于观察者模式
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const;

    // 玩家受到伤害时(蓝图中实现)
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnTakeDamage();

protected:
    virtual void NativeOnInitialized() override;

private:
    // 玩家血量变化时
    void OnHealthChanged(float Health, float HealthDelta);

    // 玩家重生时, Pawn会改变
    void OnNewPawn(APawn* NewPawn);
};
