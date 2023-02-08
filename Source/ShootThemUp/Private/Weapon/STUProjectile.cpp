// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"

ASTUProjectile::ASTUProjectile() {
    PrimaryActorTick.bCanEverTick = false;

    // ����������ײ�����
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    SetRootComponent(CollisionComponent);
}

void ASTUProjectile::BeginPlay() {
    Super::BeginPlay();
}
