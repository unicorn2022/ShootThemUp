// Shoot Them Up Game, All Rights Reserved

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

USTUWeaponFXComponent::USTUWeaponFXComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit) {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
}
