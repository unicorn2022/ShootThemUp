// Shoot Them Up Game, All Rights Reserved

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "BrainComponent.h"
#include "UI/STUHealthBarWidget.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("STUWeaponComponent")) {
    // 不自动生成Controller, 而是沿用之前回合的Controller
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();

    // 设置character的旋转
    bUseControllerRotationYaw = false;
    if (GetCharacterMovement()) {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }

    // 创建血量条组件
    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("STUHealthBarWidget");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAICharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    UpdateHealthWidgetVisibility();
}

void ASTUAICharacter::BeginPlay() {
    Super::BeginPlay();

    check(HealthWidgetComponent);
}

void ASTUAICharacter::OnDeath() {
    Super::OnDeath();

    // 角色死亡时, 清空行为树
    const auto STUController = Cast<AAIController>(Controller);
    if (STUController && STUController->BrainComponent) {
        STUController->BrainComponent->Cleanup();
    }
}

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta) {
    Super::OnHealthChanged(Health, HealthDelta);

    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (!HealthBarWidget) return;
    HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

// 更新血量条的可见性
void ASTUAICharacter::UpdateHealthWidgetVisibility() {
    if (!GetWorld()) return;
    const auto PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;
    // 角色死亡时, 要根据 SpectatorPawn 的位置确定可见性
    const auto PlayerPawn = PlayerController->GetPawnOrSpectator();
    if (!PlayerPawn) return;

    const auto PlayerLocation = PlayerPawn->GetActorLocation();
    const auto AILocation = GetActorLocation();
    const auto Distance = FVector::Distance(PlayerLocation, AILocation);
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}
