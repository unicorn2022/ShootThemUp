// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class USTUWeaponFXComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor {
    GENERATED_BODY()

public:
    ASTUProjectile();

    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
    // 榴弹的碰撞体
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    USphereComponent* CollisionComponent;

    // 榴弹的运动组件
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    UProjectileMovementComponent* MovementComponent;

    // 武器的特效
    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

protected:
    // 榴弹的爆炸半径
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageRadius = 200.0f;

    // 榴弹的爆炸伤害
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 50.0f;

    // 榴弹是否对整个爆炸范围造成相同伤害
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool DoFullDamage = false;
        
    // 榴弹的最长存活时间
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float LifeSeconds = 5.0f;
        
protected:
    virtual void BeginPlay() override;

private:
    // 榴弹的发射方向
    FVector ShotDirection;

    // 榴弹的碰撞响应函数
    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
    // 获取发射榴弹的pawn
    AController* GetController() const;
};
