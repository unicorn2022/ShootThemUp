// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon {
    GENERATED_BODY()
public:
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    // 自动开火的时间间隔
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;
    // 子弹的随机偏移角度
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    // 武器造成的伤害
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount = 10.0f;

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
