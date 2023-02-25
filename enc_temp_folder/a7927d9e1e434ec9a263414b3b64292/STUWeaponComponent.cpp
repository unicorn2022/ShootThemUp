// Shoot Them Up Game, All Rights Reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay() {
    Super::BeginPlay();

    checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can only hold %i weapons"), WeaponNum);

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

    for (auto OneWeaponData : WeaponData) {
        // 生成actor
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
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
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num()) {
        UE_LOG(LogSTUWeaponComponent, Warning, TEXT("Invalid Weapon Index!!!"));
        return;
    }

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
    const auto CurrentWeaponData =
        WeaponData.FindByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

    // 播放更换武器的动画
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

// 开火
void USTUWeaponComponent::StartFire() {
    if (!CanFire()) return;
    CurrentWeapon->StartFire();
}
// 停止开火
void USTUWeaponComponent::StopFire() {
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

// 切换武器
void USTUWeaponComponent::NextWeapon() {
    if (!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}
// 切换弹夹
void USTUWeaponComponent::Reload() {
    if (!CanReload()) return;
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

// 播放动画蒙太奇
void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) {
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}
// 初始化动画通知
void USTUWeaponComponent::InitAnimation() {
    // 订阅动画通知：切换武器
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify) {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    } else {
        UE_LOG(LogSTUWeaponComponent, Error, TEXT("Equip animation notify is forgotten to set"));
        checkNoEntry();
    }

    // 订阅动画通知：切换弹夹
    for (auto OneWeaponData : WeaponData) {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify) {
            UE_LOG(LogSTUWeaponComponent, Error, TEXT("Reload animation notify is forgotten to set"));
            checkNoEntry();
        }
        UE_LOG(LogSTUWeaponComponent, Display, TEXT("订阅 ReloadFinished 事件"));
        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
}
// 动画通知回调：切换武器
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent) {
    // 不是当前Character, 则不响应该事件
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComponent) return;

    EquipAnimInProgress = false;
}
// 动画通知回调：切换弹夹
void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent) {
    // 不是当前Character, 则不响应该事件
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComponent) return;

    ReloadAnimInProgress = false;
    // 补充弹药
    if (CurrentWeapon) CurrentWeapon->ChangeClip();
}

bool USTUWeaponComponent::CanFire() const {
    // 有武器 && 可以开火 && 没有在更换武器 && 没有在更换弹夹
    return CurrentWeapon && CurrentWeapon->CanFire() && !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const {
    // 没有在更换武器 && 没有在更换弹夹
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanReload() const {
    // 有武器 && 没有在更换武器 && 没有在更换弹夹 && 当前武器仍有剩余弹夹
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->HasClip();
}

// 获取武器UI数据
bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const {
    if (!CurrentWeapon) return false;
    UIData = CurrentWeapon->GetUIData();
    return true;
}

// 获取武器UI数据
bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const {
    if (!CurrentWeapon) return false;
    AmmoData = CurrentWeapon->GetAmmoData();
    return true;
}

// 尝试添加弹药
bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount) {
    for (const auto Weapon : Weapons) {
        if (Weapon && Weapon->IsA(WeaponType)) {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

// 判断是否需要拾取弹药
bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType) {
    for (const auto Weapon : Weapons) {
        if (Weapon && Weapon->IsA(WeaponType)) {
            return !Weapon->IsAmmoFull();
        }
    }
    return false;
}