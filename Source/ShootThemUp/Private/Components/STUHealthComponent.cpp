// Shoot Them Up Game, All Rights Reserved


#include "Components/STUHealthComponent.h"

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    
    Health = MaxHealth;
}