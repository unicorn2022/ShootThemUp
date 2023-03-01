// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUHealthComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter {
    GENERATED_BODY()

public:
    // 由于CharacterMovementComponent组件是默认组件, 因此我们需要通过参数显式指定
    ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:    
    // 组件：血量管理
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUHealthComponent* HealthComponent;

    // 组件：武器管理
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUWeaponComponent* WeaponComponent;
    
    // 死亡动画蒙太奇
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    
    // 死亡后显示的时间
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;
    // 坠落伤害速度范围
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocityScale = FVector2D(900.0f, 1200.0f);
    // 坠落伤害范围
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageScale = FVector2D(10.0f, 100.0f);
    // 角色材质的颜色属性名
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FName MaterialColorName = "Paint Color";

    virtual void BeginPlay() override;
    // 死亡回调函数
    virtual void OnDeath();


public:
    virtual void Tick(float DeltaTime) override;

    // 判断角色是否处于奔跑状态
    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsRunning() const;

    // 获取角色移动的方向
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    // 设置角色的颜色
    void SetPlayerColor(const FLinearColor& Color);

private:
    // 血量变化回调函数
    void OnHealthChanged(float Health, float HealthDelta);

    // 坠落回调函数
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

};
