// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;
class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent {
    GENERATED_BODY()

public:
    // 角色死亡委托
    FOnDeath OnDeath;
    // 角色血量变化委托
    FOnHealthChanged OnHealthChanged;

public:
    USTUHealthComponent();

    // 获取角色当前生命值
    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealth() const { return Health; }

    // 获取角色当前生命值百分比
    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return Health / MaxHealth; }

    // 判断角色是否死亡
    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    // 尝试增加血量
    bool TryToAddHealth(float HealthAmount);
    // 判断角色当前血量是否已满
    bool IsHealthFull() const;

protected:
    // 角色最大血量
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    // 角色自动治疗
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 1.0f;  // 每隔1.0s治疗一次
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 3.0f;  // 初次启动治疗间隔3.0s
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 5.0f;  // 每次治疗恢复5.0血量

    // 针对不同位置, 造成不同伤害
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    TMap<UPhysicalMaterial*, float> DamageModifiers;

    // 相机抖动
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    TSubclassOf<UCameraShakeBase> CameraShake;

    virtual void BeginPlay() override;

private:
    // 角色当前剩余血量
    float Health = 0.0f;
    // 角色自动治疗计时器
    FTimerHandle HealTimerHandle;

    // 委托：角色受到伤害
    UFUNCTION()
    void OnTakeAnyDamageHandler(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    // 委托：角色受到点伤害
    UFUNCTION()
    void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
        class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
        AActor* DamageCauser);

    // 委托：角色受到范围伤害
    UFUNCTION()
    void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo,
        class AController* InstigatedBy, AActor* DamageCauser);


    // 角色自动恢复
    void HealUpdate();

    // 设置角色血量
    void SetHealth(float NewHealth);

    // 相机抖动
    void PlayCameraShake();

    // 被杀死
    void Killed(AController* KillerController);
    // 造成伤害
    void ApplyDamage(float Damage, AController* InstigatedBy);
    // 获取击中某个骨骼需要造成的伤害修正
    float GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName);

    // 将受到的伤害传递给感官系统
    void ReportDamageEvent(float Damage, AController* InstigatedBy);
};
