// Shoot Them Up Game, All Rights Reserved


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"

ASTUAIController::ASTUAIController() {
    // ����AI��֪���
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
    SetPerceptionComponent(*STUAIPerceptionComponent);
}

void ASTUAIController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);
	
	// ִ��AI����Ϊ��
    const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
    if (STUCharacter) {
        RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
    }
}

void ASTUAIController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    
    // �ҵ�����ĵ���, ��׼��
    const auto AimActor = STUAIPerceptionComponent->GetClosetEnemy();
    SetFocus(AimActor);
}
