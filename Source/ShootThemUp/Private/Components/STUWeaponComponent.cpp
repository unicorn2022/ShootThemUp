// Shoot Them Up Game, All Rights Reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUWeaponComponent, All, All);

USTUWeaponComponent::USTUWeaponComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay() {
    Super::BeginPlay();

    // 生成武器
    SpawnWeapon();
}

// 生成武器
void USTUWeaponComponent::SpawnWeapon() {
    if (!GetWorld()) return;
    
    // 判断角色是否存在
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;
    
    // 生成actor
    CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    if (!CurrentWeapon) return;

    // 将actor绑定到角色身上
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
    UE_LOG(LogSTUWeaponComponent, Warning, TEXT("Create Weapon Success!"));
}

// 开火
void USTUWeaponComponent::Fire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->Fire();
}