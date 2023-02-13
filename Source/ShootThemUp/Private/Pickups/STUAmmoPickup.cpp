// Shoot Them Up Game, All Rights Reserved

#include "Pickups/STUAmmoPickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn) {
    UE_LOG(LogSTUAmmoPickup, Warning, TEXT("Ammo was taken"));
    return true;
}
