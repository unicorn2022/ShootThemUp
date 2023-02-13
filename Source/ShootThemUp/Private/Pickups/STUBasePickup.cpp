// Shoot Them Up Game, All Rights Reserved

#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUBasePickup, All, All);

ASTUBasePickup::ASTUBasePickup() {
    PrimaryActorTick.bCanEverTick = true;

    // 创建碰撞体组件
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);
}

void ASTUBasePickup::BeginPlay() {
    Super::BeginPlay();

    check(CollisionComponent);
}

// 两个Actor重叠在一起
void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor) {
    Super::NotifyActorBeginOverlap(OtherActor);

    // 将拾取物给到角色, 才能将拾取物拾起
    const auto Pawn = Cast<APawn>(OtherActor);
    if (GivePickupTo(Pawn)) {
        PickupWasTaken();
    }
}

void ASTUBasePickup::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

// 将拾取物给到角色, 用于修改角色属性
bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn) {
    return false;
}

// 当前Actor被拾起
void ASTUBasePickup::PickupWasTaken() {
    // 取消Actor的碰撞响应
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    
    // 将Actor设为不可见
    if (GetRootComponent()) GetRootComponent()->SetVisibility(false, true);

    // 开启重新生成Actor的定时器
    FTimerHandle RespawnTimerHandle;
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}

// 重新生成Actor
void ASTUBasePickup::Respawn() {
    // 开启Actor的碰撞响应
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    // 将Actor设为可见
    if (GetRootComponent()) GetRootComponent()->SetVisibility(true, true);
}
