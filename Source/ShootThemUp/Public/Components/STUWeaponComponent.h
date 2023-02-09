// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent {
    GENERATED_BODY()

public:
    USTUWeaponComponent();

    // 开火
    void StartFire();
    // 停止开火
    void StopFire();

    // 切换武器
    void NextWeapon();

protected:
    // 武器的类别
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<ASTUBaseWeapon>> WeaponClasses;
    // 手持武器绑定的插槽名称
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";
    // 背后武器绑定的插槽名称
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAmorySocketName = "AmorySocket";

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    // 当前武器
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    // 所有的武器
    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    // 当前武器指针
    int32 CurrentWeaponIndex = 0;

    // 生成武器
    void SpawnWeapons();
    // 将武器绑定到角色的某个插槽上
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    // 装备武器到角色手上
    void EquipWeapon(int32 WeaponIndex);
};
