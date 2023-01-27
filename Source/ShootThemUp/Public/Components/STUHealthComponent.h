// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

// 声明FOnDeath委托
DECLARE_MULTICAST_DELEGATE(FOnDeath);
// 声明FOnHealthChanged委托
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUHealthComponent();
    float GetHealth() const { return Health; }

    // 判断角色是否死亡
    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return Health <= 0.0f; }

    // 角色死亡委托
    FOnDeath OnDeath;

    // 角色血量变化委托
    FOnHealthChanged OnHealthChanged;

protected:
    // 角色最大血量
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    virtual void BeginPlay() override;

private:
    // 角色当前剩余血量
    float Health = 0.0f;

    // 角色受到伤害的回调函数
    UFUNCTION()
    void OnTakeAnyDamageHandler(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
