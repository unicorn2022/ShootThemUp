// Shoot Them Up Game, All Rights Reserved

#include "AI/Services/STUFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"

USTUFindEnemyService::USTUFindEnemyService() {
    NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return;

    const auto Controller = OwnerComp.GetAIOwner();
    const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);
    if (!PerceptionComponent) return;

    Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosetEnemy());

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
