// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent {
    GENERATED_BODY()

public:
    USTUWeaponComponent();

    // 开火
    virtual void StartFire();
    // 停止开火
    void StopFire();

    // 切换武器
    virtual void NextWeapon();

    // 切换弹夹
    void Reload();

    // 获取武器UI数据
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    // 获取武器弹药库数据
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    // 尝试添加弹药
    bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);

    // 判断是否需要拾取弹药
    bool NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType);

    // 缩放
    void Zoom(bool Enabled);

protected:
    // 武器的相关数据: 武器类, 切换弹夹的动画
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;
    // 手持武器绑定的插槽名称
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";
    // 背后武器绑定的插槽名称
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAmorySocketName = "ArmoySocket";

    // 更换装备的动画蒙太奇
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
    // 所有的武器
    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;
    // 当前武器
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;
    // 当前武器指针
    int32 CurrentWeaponIndex = 0;
    // 当前武器切换弹夹的动画
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

private:
    // 是否正在更换武器
    bool EquipAnimInProgress = false;
    // 是否正在更换弹夹
    bool ReloadAnimInProgress = false;

private:
    // 生成武器
    void SpawnWeapons();
    // 将武器绑定到角色的某个插槽上
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

    // 播放动画蒙太奇
    void PlayAnimMontage(UAnimMontage* Animation);
    // 初始化动画通知
    void InitAnimation();
    // 动画通知回调：切换武器
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    // 动画通知回调：切换弹夹
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

protected:
    // 装备武器到角色手上
    void EquipWeapon(int32 WeaponIndex);

    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;
};
