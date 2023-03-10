// Shoot Them Up Game, All Rights Reserved

#include "Pickups/STUAmmoPickup.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn) {
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if (!WeaponComponent) return false;
    
    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
