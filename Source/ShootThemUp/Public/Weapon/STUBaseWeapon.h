// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor {
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    // 开火, 不同武器会有不同的开火方式
    virtual void Fire();

protected:
    // 武器的骨骼网格体
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;
    virtual void BeginPlay() override;
};
