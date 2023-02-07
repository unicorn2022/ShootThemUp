// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

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

// 开火
void ASTUBaseWeapon::Fire() {
    UE_LOG(LogSTUBaseWeapon, Display, TEXT("Fire with Basic Weapon"));
    MakeShot();
}

// 发射子弹
void ASTUBaseWeapon::MakeShot() {
    if (!GetWorld()) return;

    // 获取武器的所有者: 即玩家
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return;
    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller) return;

    // 获取玩家的位置和朝向
    FVector ViewLocation;
    FRotator ViewRotation;
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    // 子弹路径为: 角色当前位置 -> 角色面朝方向
    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const FVector TraceStart = ViewLocation;                // SocketTransform.GetLocation();
    const FVector ShootDirection = ViewRotation.Vector();   // SocketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    // 获取子弹路径上，第一个碰撞到的对象
    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(GetOwner());   // 忽略自己
    FHitResult HitResult;                               // 碰撞的结果
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

    if (HitResult.bBlockingHit) {
        // 绘制子弹的路径: 枪口位置 -> 碰撞点
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        // 在碰撞处绘制一个球
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        
        // 显示碰撞到了哪个骨骼上, 可以通过这个信息对角色造成不同的伤害
        UE_LOG(LogSTUBaseWeapon, Display, TEXT("Fire hit bone: %s"), *HitResult.BoneName.ToString());
    } else {
        // 绘制子弹的路径: 枪口位置 -> 子弹路径的终点
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}

