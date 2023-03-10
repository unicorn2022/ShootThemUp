// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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

/* 发射子弹 */
// 当前武器可以开火
bool ASTUBaseWeapon::CanFire() {
    return !IsClipEmpty();
}
void ASTUBaseWeapon::StartFire() {}
void ASTUBaseWeapon::StopFire() {}
void ASTUBaseWeapon::MakeShot() {}

// 获取玩家的位置和朝向
bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const {
    const auto STUCharacter = Cast<ACharacter>(GetOwner());
    if (!STUCharacter) return false;

    // 如果为玩家控制, 则返回玩家的朝向
    if (STUCharacter->IsPlayerControlled()) {
        const auto Controller = STUCharacter->GetController<APlayerController>();
        if (!Controller) return false;
        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    } 
    // 如果为AI控制, 则返回枪口的朝向
    else {
        ViewLocation = GetMuzzleWorldLocation();
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }

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
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

    // 获取子弹路径上，第一个碰撞到的对象，存储到HitResult中
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}


/* 减少子弹 */
// 每次发射后减少子弹
void ASTUBaseWeapon::DecreaseAmmo() {
    if (CurrentAmmo.Bullets <= 0) return;
    CurrentAmmo.Bullets--;
}
// 判断弹药库是否为空
bool ASTUBaseWeapon::IsAmmoEmpty() const {
    return !CurrentAmmo.Infinite && CurrentAmmo.Bullets <= 0 && CurrentAmmo.Clips <= 0;
}
// 判断弹夹是否为空
bool ASTUBaseWeapon::IsClipEmpty() const {
    return CurrentAmmo.Bullets <= 0;
}
// 判断是否有剩余弹夹
bool ASTUBaseWeapon::HasClip() const {
    return CurrentAmmo.Clips > 0;
}

// 切换弹夹
void ASTUBaseWeapon::ChangeClip() {
    // 没有剩余弹药, 则直接返回
    if (IsAmmoEmpty()) return;

    // 更换弹夹, 并减少弹夹数
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    if (!CurrentAmmo.Infinite) CurrentAmmo.Clips--;
    // UE_LOG(LogSTUBaseWeapon, Display, TEXT("------ Change Clip ------"));
}
// 将弹药库信息显示到控制台
void ASTUBaseWeapon::LogAmmo() const {
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogSTUBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}


/* 补充弹药 */
// 判断弹药是否已满
bool ASTUBaseWeapon::IsAmmoFull() const {
    return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}
// 尝试添加弹药
bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount) {
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;
    LogAmmo();

    const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
    // 拾取的弹药可以补全弹药库, 则全部补全
    if(NextClipsAmount > DefaultAmmo.Clips){
        UE_LOG(LogSTUBaseWeapon, Display, TEXT("补全弹夹&弹药"));
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        CurrentAmmo.Clips = DefaultAmmo.Clips;
    } 
    // 拾取的弹药不能补全弹药库, 则优先补充弹夹
    else {
        UE_LOG(LogSTUBaseWeapon, Display, TEXT("补全弹夹"));
        CurrentAmmo.Clips = NextClipsAmount;
    }
    return true;
}

// 生成枪口特效
UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX() {
    return UNiagaraFunctionLibrary::SpawnSystemAttached(
        MuzzleFX,
        WeaponMesh,
        MuzzleSocketName,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::SnapToTarget, true);
}