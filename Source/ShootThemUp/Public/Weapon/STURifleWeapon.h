// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon {
    GENERATED_BODY()

public:
    ASTURifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    // 自动开火的时间间隔
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;
    // 子弹的随机偏移角度
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;

    // 武器造成的伤害
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.0f;

    // 武器的特效
    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;
       
protected:
    virtual void BeginPlay() override;
    // 发射子弹
    virtual void MakeShot() override;
    // 获取子弹的逻辑路径
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
    // 自动开火的定时器
    FTimerHandle ShotTimerHandle;
    // 对子弹击中的玩家进行伤害
    void MakeDamage(const FHitResult& HitResult);
};
