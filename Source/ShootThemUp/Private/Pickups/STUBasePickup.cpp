// Shoot Them Up Game, All Rights Reserved

#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

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

    GenerateRotationYaw();
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
    AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));
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

    // 播放拾取音效
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
}

// 重新生成Actor
void ASTUBasePickup::Respawn() {
    GenerateRotationYaw();
    // 开启Actor的碰撞响应
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    // 将Actor设为可见
    if (GetRootComponent()) GetRootComponent()->SetVisibility(true, true);
}

// 随机生成一个旋转速度
void ASTUBasePickup::GenerateRotationYaw() {
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
    RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}

// 可以被捡起
bool ASTUBasePickup::CouldBeTaken() const {
    // return !GetWorldTimerManager().IsTimerActive(RespawnTimeHandle);
    return CouldBeTakenTest;
}