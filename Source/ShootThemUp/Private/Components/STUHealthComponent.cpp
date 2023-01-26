// Shoot Them Up Game, All Rights Reserved


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Dev/STUFireDamageType.h"
#include "Dev/STUIceDamageType.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    
    Health = MaxHealth;

    // 订阅OnTakeAnyDamage事件
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner) {
        UE_LOG(LogSTUHealthComponent, Warning, TEXT("订阅OnTakeAnyDamage事件"));
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandler);
    }
}

// 角色受到伤害的回调函数
void USTUHealthComponent::OnTakeAnyDamageHandler(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) 
{
    Health -= Damage;
    UE_LOG(LogSTUHealthComponent, Display, TEXT("Damage: %f"), Damage);

    if (DamageType) {
        if (DamageType->IsA<USTUFireDamageType>()) {
            UE_LOG(LogSTUHealthComponent, Display, TEXT("So hot !!!!"));
        } else if (DamageType->IsA<USTUIceDamageType>()) {
            UE_LOG(LogSTUHealthComponent, Display, TEXT("So cold !!!!"));
        }
    }
}