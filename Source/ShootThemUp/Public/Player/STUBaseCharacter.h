// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter {
    GENERATED_BODY()

public:
    // 由于CharacterMovementComponent组件是默认组件, 因此我们需要通过参数显式指定
    ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:
    // 相机的弹簧臂
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;
    
    // 相机
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    // 角色的血量管理
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUHealthComponent* HealthComponent;

    // 显示角色的血量
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    // 死亡动画蒙太奇
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 判断角色是否处于奔跑状态
    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    // 获取角色移动的方向
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

private:
    // WS控制角色前后移动
    bool IsMovingForward = false;
    void MoveForward(float Amount);
    // AD控制角色左右移动
    void MoveRight(float Amount);

    // 左Shift控制角色开始跑动
    bool WantsToRun = false; // 按下Shift只能表示想要跑步, 只有当还按下W时, 才能开始跑步
    void OnStartRunning();
    void OnStopRunning();

    // 角色死亡回调函数
    void OnDeath();
    // 角色血量变化回调函数
    void OnHealthChanged(float Health);
};
