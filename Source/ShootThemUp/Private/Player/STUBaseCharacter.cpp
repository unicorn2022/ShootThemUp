// Shoot Them Up Game, All Rights Reserved

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseCharacter, All, All);

// 由于CharacterMovementComponent组件是默认组件, 因此我们需要通过参数显式指定
// 在调用父类的构造函数时, 显式指定CharacterMovementComponentName使用自定义的USTUCharacterMovementComponent
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit) 
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) 
{
    // 允许该character每一帧调用Tick()
    PrimaryActorTick.bCanEverTick = true;

    // 创建弹簧臂组件, 并设置其父组件为根组件, 允许pawn控制旋转
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    // 创建相机组件, 并设置其父组件为弹簧臂组件
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    // 创建血量组件, 由于其是纯逻辑的, 不需要设置父组件
    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("STUHealthComponent");

    // 创建血量显示组件, 并设置其父组件为根组件
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
}

void ASTUBaseCharacter::BeginPlay() {
    Super::BeginPlay();

    // 检查组件是否成功创建(仅开发阶段可用)
    check(HealthComponent);
    check(HealthTextComponent);
}

void ASTUBaseCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    // 获取角色当前血量并显示
    const float Health = HealthComponent->GetHealth();
    const FString HealthString = FString::Printf(TEXT("%.0f"), Health);
    HealthTextComponent->SetText(FText::FromString(HealthString));
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // WASD控制角色移动
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    
    // 鼠标控制相机移动
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);

    // 空格键控制角色跳跃
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    
    // 左Shift控制角色开始跑动 
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
}

// 判断角色是否处于奔跑状态
bool ASTUBaseCharacter::IsRunning() const {
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

// 获取角色移动的方向
float ASTUBaseCharacter::GetMovementDirection() const {
    // 特判: 速度为0
    if (GetVelocity().IsZero()) return 0.0f;

    const FVector VelocityDirection = GetVelocity().GetSafeNormal();
    const FVector ForwardDirection = GetActorForwardVector();
    
    // 通过点乘, 获得具体的角度值
    float angle = FMath::Acos(FVector::DotProduct(ForwardDirection, VelocityDirection));
    angle = FMath::RadiansToDegrees(angle);
    
    // 通过叉乘结果的Z值, 判断是处于顺时针还是逆时针方向
    const FVector CrossProduct = FVector::CrossProduct(ForwardDirection, VelocityDirection);
    
    // 特判: 速度与角色运动方向重合/相反
    if (CrossProduct.IsZero()) return angle;

    angle *= FMath::Sign(CrossProduct.Z);
    return angle;
}

// WS控制角色前后移动
void ASTUBaseCharacter::MoveForward(float Amount) {
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);
}
// AD控制角色左右移动
void ASTUBaseCharacter::MoveRight(float Amount) {
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
}

// 左Shift控制角色开始跑动
void ASTUBaseCharacter::OnStartRunning() {
    WantsToRun = true;
}
void ASTUBaseCharacter::OnStopRunning() {
    WantsToRun = false;
}
