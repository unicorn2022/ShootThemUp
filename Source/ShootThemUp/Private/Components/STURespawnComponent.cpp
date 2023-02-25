// Shoot Them Up Game, All Rights Reserved

#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

// 一段时间后重生
void USTURespawnComponent::Respawn(int32 RespawnTime) {
    if (!GetWorld()) return;

    RespawnCountDown = RespawnTime;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f, true);
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
