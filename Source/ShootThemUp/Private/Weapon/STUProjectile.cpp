// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

ASTUProjectile::ASTUProjectile() {
    PrimaryActorTick.bCanEverTick = false;

    // 创建球形碰撞体组件
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CollisionComponent->bReturnMaterialOnMove = true;
    SetRootComponent(CollisionComponent);

    // 创建子弹运动组件
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;

    // 创建特效组件
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTUProjectile::BeginPlay() {
    Super::BeginPlay();

    check(MovementComponent);
    check(CollisionComponent);
    check(WeaponFXComponent);

    // 对子弹运动组件进行配置
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;

    // 设置碰撞响应事件
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);

    // 设置榴弹的存活周期, 从而自动销毁
    SetLifeSpan(LifeSeconds);
}

// 榴弹的碰撞响应函数
void ASTUProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
    if (!GetWorld()) return;

    // 停止榴弹的运动
    MovementComponent->StopMovementImmediately();

    // 造成球形伤害
    UGameplayStatics::ApplyRadialDamage(GetWorld(),  // 当前世界的指针
        DamageAmount,                                // 基础伤害
        GetActorLocation(),                          // 球形伤害的中心
        DamageRadius,                                // 球形伤害的半径
        UDamageType::StaticClass(),                  // 球形伤害的类型
        {GetOwner()},                                // 球形伤害忽略的actor
        this,                                        // 造成伤害的actor
        GetController(),                             // 造成伤害的actor的controller
        DoFullDamage);                               // 是否对整个爆炸范围造成相同伤害

    // 绘制榴弹的爆炸范围
    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
    
    // 播放击中特效
    WeaponFXComponent->PlayImpactFX(Hit);

    // 销毁Actor
    Destroy();
}

// 获取发射榴弹的pawn
AController* ASTUProjectile::GetController() const {
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
