// Shoot Them Up Game, All Rights Reserved

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseCharacter, All, All);

// 由于CharacterMovementComponent组件是默认组件, 因此我们需要通过参数显式指定
// 在调用父类的构造函数时, 显式指定CharacterMovementComponentName使用自定义的USTUCharacterMovementComponent
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
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

    // 创建血量组件, 由于其是纯逻辑的, 不需要设置父组件
    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("STUHealthComponent");

    // 创建血量显示组件, 并设置其父组件为根组件
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);
}

void ASTUBaseCharacter::BeginPlay() {
    Super::BeginPlay();

    // 检查组件是否成功创建(仅开发阶段可用)
    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    // 订阅OnDeath委托
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    // 订阅OnHealthChanged委托
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
    // 先调用一次OnHealthChanged, 获取角色的初始血量
    OnHealthChanged(HealthComponent->GetHealth());

    // 订阅LandedDelegate委托
    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);

    // 生成武器
    SpawnWeapon();
}

void ASTUBaseCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

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

// 死亡回调函数
void ASTUBaseCharacter::OnDeath() {
    UE_LOG(LogSTUBaseCharacter, Warning, TEXT("Player %s is dead"), *GetName());
    // 播放死亡动画蒙太奇
    PlayAnimMontage(DeathAnimMontage);
    // 禁止角色的移动
    GetCharacterMovement()->DisableMovement();
    // 一段时间后摧毁角色
    SetLifeSpan(LifeSpanOnDeath);
    // 切换状态, 从而将pawn切换为观察者类
    if (Controller) {
        Controller->ChangeState(NAME_Spectating);
    }
}

// 血量变化回调函数
void ASTUBaseCharacter::OnHealthChanged(float Health) {
    // 获取角色当前血量并显示
    const FString HealthString = FString::Printf(TEXT("%.0f"), Health);
    HealthTextComponent->SetText(FText::FromString(HealthString));
}

// 坠落回调函数
void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit) {
    // 根据角色坠落时的Z速度, 计算扣除血量
    const float FallVelocityZ = FMath::Abs(GetCharacterMovement()->Velocity.Z);
    UE_LOG(LogSTUBaseCharacter, Display, TEXT("On Landed: %f"), FallVelocityZ);
    if (FallVelocityZ < LandedDamageVelocityScale.X) return;

    // 将FallVelocityZ映射到(LandedDamageVelocityScale => LandedDamageScale)
    const float FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocityScale, LandedDamageScale, FallVelocityZ);
    UE_LOG(LogSTUBaseCharacter, Display, TEXT("FinalDamage: %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

// 生成武器
void ASTUBaseCharacter::SpawnWeapon() {
    if (!GetWorld()) return;
    // 生成actor
    const auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    // 将actor绑定到角色身上
    if (Weapon) {
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
        Weapon->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket");
    }
}
