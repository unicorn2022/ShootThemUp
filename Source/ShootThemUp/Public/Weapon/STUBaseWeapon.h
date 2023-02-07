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

    // 武器枪口的插槽名称
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    // 子弹的最大路程
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TraceMaxDistance = 1500;

    // 发射子弹
    void MakeShot();
};
