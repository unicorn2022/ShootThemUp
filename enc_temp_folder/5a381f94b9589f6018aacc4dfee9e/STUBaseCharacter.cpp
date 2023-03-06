// Shoot Them Up Game, All Rights Reserved

#include "Player/STUBaseCharacter.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseCharacter, All, All);

// 由于CharacterMovementComponent组件是默认组件, 因此我们需要通过参数显式指定
// 在调用父类的构造函数时, 显式指定CharacterMovementComponentName使用自定义的USTUCharacterMovementComponent
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
    // 允许该character每一帧调用Tick()
    PrimaryActorTick.bCanEverTick = true;

    // 创建血量组件, 由于其是纯逻辑的, 不需要设置父组件
    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("STUHealthComponent");

    // 创建武器组件, 由于其是纯逻辑的, 不需要设置父组件
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("STUWeaponComponent");
}

void ASTUBaseCharacter::BeginPlay() {
    Super::BeginPlay();

    // 检查组件是否成功创建(仅开发阶段可用)
    check(HealthComponent);
    check(GetCharacterMovement());
    check(GetMesh());

    // 订阅OnDeath委托
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    // 订阅OnHealthChanged委托
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
    // 先调用一次OnHealthChanged, 获取角色的初始血量
    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);

    // 订阅LandedDelegate委托
    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

// 血量变化回调函数
void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta) {}

// 判断角色是否处于奔跑状态
bool ASTUBaseCharacter::IsRunning() const {
    return false;
}

// 死亡回调函数
void ASTUBaseCharacter::OnDeath() {
    UE_LOG(LogSTUBaseCharacter, Warning, TEXT("Player %s is dead"), *GetName());
    // 播放死亡动画蒙太奇
    // PlayAnimMontage(DeathAnimMontage);

    // 禁止角色的移动
    GetCharacterMovement()->DisableMovement();

    // 一段时间后摧毁角色
    SetLifeSpan(LifeSpanOnDeath);

    // 禁止胶囊体碰撞
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    // 停止武器组件的开火
    WeaponComponent->StopFire();

    // 启用物理模拟, 实现角色死亡效果
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
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

// 设置角色的颜色
void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color) {
    const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (!MaterialInst) return;

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}