// Shoot Them Up Game, All Rights Reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUWeaponComponent, All, All);

USTUWeaponComponent::USTUWeaponComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay() {
    Super::BeginPlay();

    // ��ʼ������
    InitAnimation();
    // ��������
    SpawnWeapons();
    // װ������
    CurrentWeaponIndex = 0;
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    // ������������
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons) {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

// ��������
void USTUWeaponComponent::SpawnWeapons() {
    // �жϽ�ɫ�Ƿ����
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Character) return;
    
    for (auto WeaponClass : WeaponClasses) {
        // ����actor
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
        if (!Weapon) continue;

        // ����������������
        Weapon->SetOwner(Character); 
        Weapons.Add(Weapon);

        // �������󶨵���ɫ��ĳ�������
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponAmorySocketName);
    }
}

// �������󶨵���ɫ��ĳ�������
void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName) {
    if (!Weapon || !SceneComponent) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

// װ����������ɫ����
void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex) {
    // �жϽ�ɫ�Ƿ���� 
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Character) return;

    // ����Ѿ�������, ����ǰ����ת�Ƶ�����
    if (CurrentWeapon) {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponAmorySocketName);
    }

    // �������ϵ�����
    CurrentWeapon = Weapons[WeaponIndex];
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    
    // ���Ÿ��������Ķ���
    PlayAnimMontage(EquipAnimMontage);
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

// �л�����
void USTUWeaponComponent::NextWeapon() {
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

// ���Ŷ�����̫��
void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

// ��ʼ������֪ͨ
void USTUWeaponComponent::InitAnimation() {
    if (!EquipAnimMontage) return;
    
    const auto NotifyEvents = EquipAnimMontage->Notifies;
    for (auto NotifyEvent : NotifyEvents) {
        auto EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(NotifyEvent.Notify);
        if (EquipFinishedNotify) {
            EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
        }
    }

}
// ����֪ͨ�ص�
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Character) return;

    // ���ǵ�ǰcharacter, ����Ӧ���¼�
    if (Character->GetMesh() != MeshComponent) return;
    UE_LOG(LogSTUWeaponComponent, Display, TEXT("Equip Finished"));
}