// Shoot Them Up Game, All Rights Reserved

#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

// һ��ʱ�������
void USTURespawnComponent::Respawn(int32 RespawnTime) {
    if (!GetWorld()) return;

    RespawnCountDown = RespawnTime;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f, true);
}

// ��������
bool USTURespawnComponent::IsRespawnInProgress() const {
    if (!GetWorld()) return false;
    return GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}

void USTURespawnComponent::RespawnTimerUpdate() {
    RespawnCountDown--;
    if (RespawnCountDown <= 0) {
        if (!GetWorld()) return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (!GameMode) return;

        GameMode->RespawnResqust(Cast<AController>(GetOwner()));
    }
}
