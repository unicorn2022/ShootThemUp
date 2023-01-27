// Shoot Them Up Game, All Rights Reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay() {
    Super::BeginPlay();

    Health = MaxHealth;
    // 广播OnHealthChanged委托
    OnHealthChanged.Broadcast(Health);

    // 订阅OnTakeAnyDamage事件
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner) {
        UE_LOG(LogSTUHealthComponent, Warning, TEXT("订阅OnTakeAnyDamage事件"));
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandler);
    }
}

// 角色受到伤害的回调函数
void USTUHealthComponent::OnTakeAnyDamageHandler(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) {
    if (Damage <= 0.0f || IsDead()) return;

    // 保证Health在合理的范围内
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    // 广播OnHealthChanged委托
    OnHealthChanged.Broadcast(Health);

    // 角色死亡后, 广播OnDeath委托
    if (IsDead()) OnDeath.Broadcast();
}