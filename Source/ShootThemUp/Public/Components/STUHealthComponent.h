// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

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

    // 角色死亡委托
    FOnDeath OnDeath;

    // 角色血量变化委托
    FOnHealthChanged OnHealthChanged;

protected:
    // 角色最大血量
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    // 角色自动治疗
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 1.0f;    // 每隔1.0s治疗一次
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 3.0f;         // 初次启动治疗间隔3.0s
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 5.0f;      // 每次治疗恢复5.0血量

    virtual void BeginPlay() override;

private:
    // 角色当前剩余血量
    float Health = 0.0f;
    // 角色自动治疗计时器
    FTimerHandle HealTimerHandle;

    // 角色受到伤害的回调函数
    UFUNCTION()
    void OnTakeAnyDamageHandler(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    // 角色自动恢复
    void HealUpdate();

    // 设置角色血量
    void SetHealth(float NewHealth);
};
