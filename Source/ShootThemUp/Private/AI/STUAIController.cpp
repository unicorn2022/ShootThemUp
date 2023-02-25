// Shoot Them Up Game, All Rights Reserved

#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STURespawnComponent.h"

ASTUAIController::ASTUAIController() {
    // ����AI��֪���
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
    SetPerceptionComponent(*STUAIPerceptionComponent);

    // �����������
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponent");
    

    // ΪAI����PlayerState
    bWantsPlayerState = true;
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
    // const auto AimActor = STUAIPerceptionComponent->GetClosetEnemy();
    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const {
    if (!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
