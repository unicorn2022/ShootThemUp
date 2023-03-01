// Shoot Them Up Game, All Rights Reserved

#include "Player/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUWeaponComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit) {
    // 允许该character每一帧调用Tick()
    PrimaryActorTick.bCanEverTick = true;

    // 创建弹簧臂组件, 并设置其父组件为根组件, 允许pawn控制旋转
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    // 创建相机组件, 并设置其父组件为弹簧臂组件
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

// 绑定输入
void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

    // WASD控制角色移动
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);

    // 鼠标控制相机移动
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::AddControllerYawInput);

    // 空格键控制角色跳跃
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::Jump);

    // 左Shift控制角色开始跑动
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::OnStopRunning);

    // 鼠标左键控制武器开火
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);

    // Tab键切换武器
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);

    // R键切换弹夹
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
}

// WS控制角色前后移动
void ASTUPlayerCharacter::MoveForward(float Amount) {
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);
}
// AD控制角色左右移动
void ASTUPlayerCharacter::MoveRight(float Amount) {
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
}
// 左Shift控制角色开始跑动
void ASTUPlayerCharacter::OnStartRunning() {
    WantsToRun = true;
}
void ASTUPlayerCharacter::OnStopRunning() {
    WantsToRun = false;
}

// 判断角色是否处于奔跑状态
bool ASTUPlayerCharacter::IsRunning() const {
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

// 死亡回调函数
void ASTUPlayerCharacter::OnDeath() {
    Super::OnDeath();
    // 切换状态, 从而将pawn切换为观察者类
    if (Controller) {
        Controller->ChangeState(NAME_Spectating);
    }
}