// Shoot Them Up Game, All Rights Reserved

#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STURespawnComponent.h"

ASTUAIController::ASTUAIController() {
    // 创建AI感知组件
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
    SetPerceptionComponent(*STUAIPerceptionComponent);

    // 创建重生组件
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponent");
    

    // 为AI分配PlayerState
    bWantsPlayerState = true;
}

void ASTUAIController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);

    // 执行AI的行为树
    const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
    if (STUCharacter) {
        RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
    }
}

void ASTUAIController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    // 找到最近的敌人, 瞄准他
    // const auto AimActor = STUAIPerceptionComponent->GetClosetEnemy();
    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const {
    if (!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
