// Shoot Them Up Game, All Rights Reserved

#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUAIWeaponComponent, All, All);

void USTUAIWeaponComponent::StartFire() {

    UE_LOG(LogSTUAIWeaponComponent, Warning, TEXT("AI Start Fire"));

    // 当前武器没有弹药了: 换武器
    if (CurrentWeapon->IsAmmoEmpty()) {
        UE_LOG(LogSTUAIWeaponComponent, Warning, TEXT("AI ammo empty"));
        NextWeapon();
    }
    // 当前弹夹没有子弹了: 换弹夹
    else if (CurrentWeapon->IsClipEmpty()) {
        UE_LOG(LogSTUAIWeaponComponent, Warning, TEXT("AI clip empty"));
        Reload();
    } 
    // 当前弹夹有子弹: 开始射击
    else {
        if (!CanFire()) return;
        UE_LOG(LogSTUAIWeaponComponent, Warning, TEXT("AI fire"));
        CurrentWeapon->StartFire();
    }

}

void USTUAIWeaponComponent::NextWeapon() {
    if (!CanEquip()) return;

    UE_LOG(LogSTUAIWeaponComponent, Warning, TEXT("AI Next Weapon"));
    
    // 为防止AI无限换武器, 需要判定下一把武器有子弹, 才能更换
    int32 NextIndex  = (CurrentWeaponIndex + 1) % Weapons.Num();
    while (NextIndex != CurrentWeaponIndex) {
        if (!Weapons[NextIndex]->IsAmmoEmpty()) break;
        NextIndex = (NextIndex + 1) % Weapons.Num();
    }

    if (NextIndex != CurrentWeaponIndex) {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}
