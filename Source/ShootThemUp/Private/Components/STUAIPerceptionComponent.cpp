// Shoot Them Up Game, All Rights Reserved

#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosetEnemy() const {
    // ��ȡAI��Ұ�ڵ�����Actor
    TArray<AActor*> PerciveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerciveActors);
    if (PerciveActors.Num() == 0) return nullptr;

    // ��ȡ��ǰ��ɫ��Pawn
    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;
    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    // ��ȡ���뵱ǰ��ɫ�����Character
    float ClosetDistance = MAX_FLT;
    AActor* ClosetActor = nullptr;
    for (const auto PerciveActor : PerciveActors) {
        const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PerciveActor);
        if (!HealthComponent || HealthComponent->IsDead()) continue;
        
        const auto CurrentDistance = (PerciveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
        if (CurrentDistance < ClosetDistance) {
            ClosetDistance = CurrentDistance;
            ClosetActor = PerciveActor;
        }
    }

    return ClosetActor;
}
