// Shoot Them Up Game, All Rights Reserved

#include "Pickups/STUHealthPickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn) {
    UE_LOG(LogSTUHealthPickup, Warning, TEXT("Health was taken"));
    return true;
}
