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
    
    for (auto OneWeaponData : WeaponData) {
        // ����actor
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
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
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num()) {
        UE_LOG(LogSTUWeaponComponent, Warning, TEXT("Invalid Weapon Index!!!"));
        return;
    }

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
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) { 
        return Data.WeaponClass == CurrentWeapon->GetClass(); 
    });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    
    // ���Ÿ��������Ķ���
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

// ����
void USTUWeaponComponent::StartFire() {
    if (!CanFire()) return;
    CurrentWeapon->StartFire();
}
// ֹͣ����
void USTUWeaponComponent::StopFire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

// �л�����
void USTUWeaponComponent::NextWeapon() {
    if (!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

// �л�����
void USTUWeaponComponent::Reload() {
    PlayAnimMontage(CurrentReloadAnimMontage);
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
    // ���ǵ�ǰCharacter, ����Ӧ���¼�
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComponent) return;

    EquipAnimInProgress = false;
}

bool USTUWeaponComponent::CanFire() const {
    // ��������û�����ڸ�������
    return CurrentWeapon && !EquipAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const {
    // û�����ڸ�������
    return !EquipAnimInProgress;
}