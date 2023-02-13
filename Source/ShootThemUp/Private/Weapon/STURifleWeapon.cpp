// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTURifleWeapon, All, All);

// 开火, 不同武器会有不同的开火方式
void ASTURifleWeapon::StartFire() {
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}
// 停止开火
void ASTURifleWeapon::StopFire() {
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

// 发射子弹
void ASTURifleWeapon::MakeShot() {
    // 判断当前弹夹是否为空
    if (!GetWorld() || IsClipEmpty()) {
        StopFire();
        return;
    }

    // 获取子弹的逻辑路径
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) {
        StopFire();
        return;
    }

    // 计算子弹的碰撞结果
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit) {
        // 对子弹击中的玩家进行伤害
        MakeDamage(HitResult);

        // 绘制子弹的路径: 枪口位置 -> 碰撞点
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        // 在碰撞处绘制一个球
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

        // 显示碰撞到了哪个骨骼上, 可以通过这个信息对角色造成不同的伤害
        UE_LOG(LogSTURifleWeapon, Display, TEXT("Fire hit bone: %s"), *HitResult.BoneName.ToString());
    } else {
        // 绘制子弹的路径: 枪口位置 -> 子弹路径的终点
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }

    // 减少弹药数
    DecreaseAmmo();
}

// 获取子弹的逻辑路径
bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const {
    // 获取玩家的位置和朝向
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    // 子弹的起点为: 角色当前位置
    TraceStart = ViewLocation;
    // 子弹的方向为: 角色当前正前方 + 一个随机的偏移
    // 当角色血量很低时, 偏移角可以变大, 模拟角色打不准的情况
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    // 子弹的终点为: 角色当前位置沿子弹方向运动一定的距离
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

// 对子弹击中的玩家进行伤害
void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult) {
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor) return;

    DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}