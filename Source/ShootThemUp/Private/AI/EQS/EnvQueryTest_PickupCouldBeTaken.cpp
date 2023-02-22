// Shoot Them Up Game, All Rights Reserved

#include "AI/EQS/EnvQueryTest_PickupCouldBeTaken.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/STUBasePickup.h"

UEnvQueryTest_PickupCouldBeTaken::UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) 
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const {
    // ��õ�ǰ�������õĲ���ƥ��ֵ
    const auto DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    const bool WantsBeTakable = BoolValue.GetValue();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It) {
        const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);
        if (!PickupActor) continue;

        // �жϵ�ǰActor�Ƿ���Ա�ʰȡ
        const auto CouldBeTaken = PickupActor->CouldBeTaken();
        It.SetScore(TestPurpose, FilterType, CouldBeTaken, WantsBeTakable);
    }
}
