// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STUNextLocationTask.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUNextLocationTask : public UBTTaskNode {
    GENERATED_BODY()

public:
    USTUNextLocationTask();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float Radius = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector AimLocationKey;

    // ʼ�����Լ�Ϊ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    bool SelfCenter = true;

    // Ŀ��Actor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "!SelfCenter"))
    FBlackboardKeySelector CenterActorKey;
};
