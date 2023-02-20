// Shoot Them Up Game, All Rights Reserved

#include "AI/Tasks/STUNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask() {
    NodeName = "Generate and Set Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Type::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Type::Failed;

    const auto NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSystem) return EBTNodeResult::Type::Failed;

    // 通过NavigationSystem获取一个随机点
    FNavLocation NavLocation;
    const auto Found = NavSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, NavLocation);
    if (!Found) return EBTNodeResult::Type::Failed;

    // 设置Blackboard中的键值
    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Type::Succeeded;
}
