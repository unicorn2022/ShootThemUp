// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASTUProjectile::ASTUProjectile() {
    PrimaryActorTick.bCanEverTick = false;

    // ����������ײ�����
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    SetRootComponent(CollisionComponent);

    // �����ӵ��˶����
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;
}

void ASTUProjectile::BeginPlay() {
    Super::BeginPlay();

    // ���ӵ��˶������������
    check(MovementComponent);
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;

    // �����ӵ��Ĵ������, �Ӷ��Զ�����
    SetLifeSpan(5.0f);
}
