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
    virtual void StartFire();
    // 停止开火
    virtual void StopFire();

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

    // 武器造成的伤害
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float DamageAmount = 10.0f;

protected:
    // 发射子弹
    virtual void MakeShot();
    // 获取玩家控制器
    APlayerController* GetPlayerController() const;
    // 获取玩家的位置和朝向
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    // 获取枪口的位置
    FVector GetMuzzleWorldLocation() const;
    // 获取子弹的逻辑路径
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    // 执行碰撞逻辑
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
    // 对子弹击中的玩家进行伤害
    void MakeDamage(const FHitResult& HitResult) const;
};
