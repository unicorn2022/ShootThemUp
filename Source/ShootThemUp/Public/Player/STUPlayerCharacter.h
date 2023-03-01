// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter {
    GENERATED_BODY()

public:
    // 由于CharacterMovementComponent组件是默认组件, 因此我们需要通过参数显式指定
    ASTUPlayerCharacter(const FObjectInitializer& ObjInit);

protected:
    // 组件：相机的弹簧臂
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;
    // 组件：相机
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;
    // 组件：相机的球形碰撞体
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* CameraCollisionComponent;

    // 委托：角色死亡
    virtual void OnDeath() override;
    virtual void BeginPlay() override;

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 判断角色是否处于奔跑状态
    virtual bool IsRunning() const override;

private:
    // WS控制角色前后移动
    bool IsMovingForward = false;
    void MoveForward(float Amount);
    // AD控制角色左右移动
    void MoveRight(float Amount);

    // 左Shift控制角色开始跑动
    bool WantsToRun = false;  // 按下Shift只能表示想要跑步, 只有当还按下W时, 才能开始跑步
    void OnStartRunning();
    void OnStopRunning();

    // 委托：相机与角色开始重叠
    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    // 委托：相机与角色结束重叠
    UFUNCTION()
    void OnCameraCollisionEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    // 检查相机是否穿过角色身体
    void CheckCameraOverlap();
};
