// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor {
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    // 开火, 不同武器会有不同的开火方式
    virtual void StartFire();
    // 停止开火
    virtual void StopFire();

    FWeaponUIData GetUIData() const { return UIData; }
    FAmmoData GetAmmoData() const { return CurrentAmmo; }

protected:
    // 武器的骨骼网格体
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    // 武器枪口的插槽名称
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    // 子弹的最大路程
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500;

    // 默认弹药库
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

    // 武器的显示UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

protected:
    virtual void BeginPlay() override;
    // 发射子弹
    virtual void MakeShot();
    // 获取玩家控制器
    APlayerController* GetPlayerController() const;
    // 获取玩家的位置和朝向
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    // 获取枪口的位置
    FVector GetMuzzleWorldLocation() const;
    // 获取子弹的逻辑路径
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    // 执行碰撞逻辑
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

    // 每次发射后减少弹药
    void DecreaseAmmo();
    // 判断弹药库是否为空
    bool IsAmmoEmpty() const;
    // 判断弹夹是否为空
    bool IsClipEmpty() const;
    // 切换弹夹
    void ChangeClip();
    // 将弹药库信息显示到控制台
    void LogAmmo() const;

private:
    // 当前弹药库
    FAmmoData CurrentAmmo;
};
