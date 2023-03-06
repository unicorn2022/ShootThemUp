// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTURifleWeapon, All, All);

ASTURifleWeapon::ASTURifleWeapon() {
    // 创建特效组件
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay() {
    Super::BeginPlay();

    check(WeaponFXComponent);
}

// 开火, 不同武器会有不同的开火方式
void ASTURifleWeapon::StartFire() {
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    InitMuzzleFX();
}
// 停止开火
void ASTURifleWeapon::StopFire() {
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    SetMuzzleFXVisibility(false);
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

    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit) {
        // 对子弹击中的玩家进行伤害
        MakeDamage(HitResult);
        
        // 击中时, 子弹路径为: 枪口位置 -> 碰撞点
        TraceFXEnd = HitResult.ImpactPoint;
        
        // 播放击中特效
        WeaponFXComponent->PlayImpactFX(HitResult);
    }

    // 减少弹药数
    DecreaseAmmo();

    // 生成子弹路径特效
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
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

    DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetController(), this);
}

// 初始化枪口特效组件
void ASTURifleWeapon::InitMuzzleFX() {
    if (!MuzzleFXComponent) {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    SetMuzzleFXVisibility(true);
}

// 设置特效的可见性
void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible) {
    if (MuzzleFXComponent) {
        MuzzleFXComponent->SetPaused(!Visible);
        MuzzleFXComponent->SetVisibility(Visible);
    }
}

// 生成子弹路径特效
void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd) {
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);

    if (TraceFXComponent) {
        // DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}

// 获取发射子弹的pawn
AController* ASTURifleWeapon::GetController() const {
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}