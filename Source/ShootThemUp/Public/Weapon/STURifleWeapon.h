// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UniagaraSystem;
class UAudioComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon {
    GENERATED_BODY()

public:
    ASTURifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;
    // 缩放
    virtual void Zoom(bool Enabled) override;

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

    // 缩放的视角
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float FOVZoomAngle = 50.0f;

    // 子弹路径特效系统
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;
    // 子弹路径特效系统中的参数名称
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";
    
    
    // 武器的特效组件
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
    
    // 枪口特效组件
    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    // 音效组件
    UPROPERTY()
    UAudioComponent* FireAudioComponent;

    // 默认缩放视角
    float DefaultCameraFOV = 90.0f;

private:
    // 对子弹击中的玩家进行伤害
    void MakeDamage(const FHitResult& HitResult);

    // 初始化特效组件
    void InitFX();
    // 设置特效的可见性
    void SetFXActive(bool IsActive);

    // 生成子弹路径特效
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

    // 获取发射子弹的pawn
    AController* GetController() const;
};
