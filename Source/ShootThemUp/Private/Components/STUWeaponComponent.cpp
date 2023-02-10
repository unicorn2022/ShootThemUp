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

    // 初始化动画
    InitAnimation();
    // 生成武器
    SpawnWeapons();
    // 装备武器
    CurrentWeaponIndex = 0;
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    // 销毁所有武器
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons) {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

// 生成武器
void USTUWeaponComponent::SpawnWeapons() {
    // 判断角色是否存在
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Character) return;
    
    for (auto WeaponClass : WeaponClasses) {
        // 生成actor
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
        if (!Weapon) continue;

        // 设置武器的所有者
        Weapon->SetOwner(Character); 
        Weapons.Add(Weapon);

        // 将武器绑定到角色的某个插槽上
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponAmorySocketName);
    }
}

// 将武器绑定到角色的某个插槽上
void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName) {
    if (!Weapon || !SceneComponent) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

// 装备武器到角色手上
void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex) {
    // 判断角色是否存在 
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Character) return;

    // 如果已经有武器, 将当前武器转移到背后
    if (CurrentWeapon) {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponAmorySocketName);
    }

    // 更换手上的武器
    CurrentWeapon = Weapons[WeaponIndex];
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    
    // 播放更换武器的动画
    PlayAnimMontage(EquipAnimMontage);
}

// 开火
void USTUWeaponComponent::StartFire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->StartFire();
}
// 停止开火
void USTUWeaponComponent::StopFire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

// 切换武器
void USTUWeaponComponent::NextWeapon() {
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

// 播放动画蒙太奇
void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

// 初始化动画通知
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
// 动画通知回调
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Character) return;

    // 不是当前character, 则不响应该事件
    if (Character->GetMesh() != MeshComponent) return;
    UE_LOG(LogSTUWeaponComponent, Display, TEXT("Equip Finished"));
}