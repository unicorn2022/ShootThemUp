// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUBasePickup {
    GENERATED_BODY()

private:
    // 将拾取物给到角色, 用于修改角色属性
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
