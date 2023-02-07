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

    // ��������
    SpawnWeapon();
}

// ��������
void USTUWeaponComponent::SpawnWeapon() {
    if (!GetWorld()) return;
    
    // �жϽ�ɫ�Ƿ����
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;
    
    // ����actor
    CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    if (!CurrentWeapon) return;

    // �������󶨵���ɫ����
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
    CurrentWeapon->SetOwner(Character); // ����������������
}

// ����
void USTUWeaponComponent::StartFire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->StartFire();
}
// ֹͣ����
void USTUWeaponComponent::StopFire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}