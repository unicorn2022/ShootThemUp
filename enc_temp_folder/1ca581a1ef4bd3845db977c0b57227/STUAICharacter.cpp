// Shoot Them Up Game, All Rights Reserved

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("STUWeaponComponent")) {
    // 将该character自动由STUAIController接管
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASTUAIController::StaticClass();

    // 设置character的旋转
    bUseControllerRotationYaw = false;
    if (GetCharacterMovement()) {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}

void ASTUAICharacter::OnDeath() {
    Super::OnDeath();

    // 角色死亡时, 清空行为树
    const auto STUController = Cast<AAIController>(Controller);
    if (STUController && STUController->BrainComponent) {
        STUController->BrainComponent->Cleanup();
    }
}
