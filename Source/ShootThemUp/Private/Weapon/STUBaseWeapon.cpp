// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon() {
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay() {
    Super::BeginPlay();
}

// ¿ª»ð
void ASTUBaseWeapon::Fire() {
    UE_LOG(LogSTUBaseWeapon, Warning, TEXT("Fire with Basic Weapon"));
}
