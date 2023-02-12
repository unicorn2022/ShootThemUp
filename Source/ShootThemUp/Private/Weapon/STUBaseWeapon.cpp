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
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets can't be <= 0"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips can't be <= 0"));
    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire() {}
void ASTUBaseWeapon::StopFire() {}
void ASTUBaseWeapon::MakeShot() {}

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
    // 子弹的方向为: 角色当前正前方
    const FVector ShootDirection = ViewRotation.Vector();
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

// 每次发射后减少子弹
void ASTUBaseWeapon::DecreaseAmmo() {
    CurrentAmmo.Bullets--;

    if (IsClipEmpty() && !IsAmmoEmpty()) ChangeClip();
}
// 判断弹药库是否为空
bool ASTUBaseWeapon::IsAmmoEmpty() const {
    return !CurrentAmmo.Infinite && CurrentAmmo.Bullets == 0 && CurrentAmmo.Clips == 0;
}
// 判断弹夹是否为空
bool ASTUBaseWeapon::IsClipEmpty() const {
    return CurrentAmmo.Bullets == 0;
}
// 切换弹夹
void ASTUBaseWeapon::ChangeClip() {
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    if (!CurrentAmmo.Infinite) CurrentAmmo.Clips--;
    UE_LOG(LogSTUBaseWeapon, Display, TEXT("------ Change Clip ------"));
}
// 将弹药库信息显示到控制台
void ASTUBaseWeapon::LogAmmo() const {
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogSTUBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}