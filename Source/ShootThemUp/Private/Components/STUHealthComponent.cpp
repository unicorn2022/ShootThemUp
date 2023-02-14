// Shoot Them Up Game, All Rights Reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay() {
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);

    // 订阅OnTakeAnyDamage事件
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner) {
        UE_LOG(LogSTUHealthComponent, Display, TEXT("订阅 OnTakeAnyDamage 事件"));
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandler);
    }
}

// 角色受到伤害的回调函数
void USTUHealthComponent::OnTakeAnyDamageHandler(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) {
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);

    // 角色受伤时, 停止自动恢复
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    
    // 角色死亡后, 广播OnDeath委托
    if (IsDead()) OnDeath.Broadcast();
    // 角色未死亡且可以自动恢复
    else if (AutoHeal) {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }

    // 相机抖动
    PlayCameraShake();
}

// 角色自动恢复
void USTUHealthComponent::HealUpdate() {
    SetHealth(Health + HealModifier);
    // 当角色的生命值最大时, 停止计时器
    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld()) {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

// 设置角色血量
void USTUHealthComponent::SetHealth(float NewHealth) {
    // 保证Health在合理的范围内
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    // 广播OnHealthChanged委托
    OnHealthChanged.Broadcast(Health);
}

// 尝试增加血量
bool USTUHealthComponent::TryToAddHealth(float HealthAmount) {
    // 角色已死亡、血量已满，均不可回血
    if (IsDead() || IsHealthFull()) return false;
    
    SetHealth(Health + HealthAmount);
    return true;
}
// 判断角色当前血量是否已满
bool USTUHealthComponent::IsHealthFull() const {
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

// 相机抖动
void USTUHealthComponent::PlayCameraShake() {
    if (IsDead()) return;

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}