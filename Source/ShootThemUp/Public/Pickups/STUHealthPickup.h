// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUBasePickup {
    GENERATED_BODY()

protected:
    // 拾取物恢复的血量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    int32 HealthAmount = 50.0f;

private:
    // 将拾取物给到角色, 用于修改角色属性
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
