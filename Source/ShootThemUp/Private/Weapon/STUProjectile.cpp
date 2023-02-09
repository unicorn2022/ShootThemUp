// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ASTUProjectile::ASTUProjectile() {
    PrimaryActorTick.bCanEverTick = false;

    // ����������ײ�����
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SetRootComponent(CollisionComponent);

    // �����ӵ��˶����
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;
}

void ASTUProjectile::BeginPlay() {
    Super::BeginPlay();

    check(MovementComponent);
    check(CollisionComponent);

    // ���ӵ��˶������������
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;

    // ������ײ��Ӧ�¼�
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);

    // �����񵯵Ĵ������, �Ӷ��Զ�����
    SetLifeSpan(LifeSeconds);
}

// �񵯵���ײ��Ӧ����
void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
    if (!GetWorld()) return;

    // ֹͣ�񵯵��˶�
    MovementComponent->StopMovementImmediately();

    // ��������˺�
    UGameplayStatics::ApplyRadialDamage(
        GetWorld(),                     // ��ǰ�����ָ��
        DamageAmount,                   // �����˺�
        GetActorLocation(),             // �����˺�������
        DamageRadius,                   // �����˺��İ뾶
        UDamageType::StaticClass(),     // �����˺�������
        {GetOwner()},                   // �����˺����Ե�actor
        this,                           // ����˺���actor
        GetController(),                // ����˺���actor��controller
        DoFullDamage);                  // �Ƿ��������ը��Χ�����ͬ�˺�

    // �����񵯵ı�ը��Χ
    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);

    // ����Actor
    Destroy();
}

// ��ȡ�����񵯵�pawn
AController* ASTUProjectile::GetController() const {
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
