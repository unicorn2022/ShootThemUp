// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon() {
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay() {
    Super::BeginPlay();
    check(WeaponMesh);
}

// 开火, 不同武器会有不同的开火方式
void ASTUBaseWeapon::StartFire() {
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTUBaseWeapon::MakeShot, TimeBetweenShots, true);
}
// 停止开火
void ASTUBaseWeapon::StopFire() {
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

// 发射子弹
void ASTUBaseWeapon::MakeShot() {
    if (!GetWorld()) return;
    
    // 获取子弹的逻辑路径
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

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
        UE_LOG(LogSTUBaseWeapon, Display, TEXT("Fire hit bone: %s"), *HitResult.BoneName.ToString());
    } else {
        // 绘制子弹的路径: 枪口位置 -> 子弹路径的终点
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}

// 获取玩家控制器
APlayerController* ASTUBaseWeapon::GetPlayerController() const {
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;
    return Player->GetController<APlayerController>();
}

// 获取玩家的位置和朝向
bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const {
    const auto Controller = GetPlayerController();
    if (!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

// 获取枪口的位置
FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const {
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

// 获取子弹的逻辑路径
bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const {
    // 获取玩家的位置和朝向
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    // 子弹的起点为: 角色当前位置
    TraceStart = ViewLocation;
    // 子弹的方向为: 角色当前正前方 + 一个随机的偏移
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    // 子弹的终点为: 角色当前位置沿子弹方向运动一定的距离
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

// 执行碰撞逻辑
void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const {
    if (!GetWorld()) return;
    
    // 忽略自己
    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(GetOwner());  
    
    // 获取子弹路径上，第一个碰撞到的对象，存储到HitResult中
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
}

// 对子弹击中的玩家进行伤害
void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult) const {
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor) return;

    DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), nullptr);
}
