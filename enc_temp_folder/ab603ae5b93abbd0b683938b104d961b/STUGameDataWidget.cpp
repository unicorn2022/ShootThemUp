// Shoot Them Up Game, All Rights Reserved

#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"

int32 USTUGameDataWidget::GetKillsNum() const {
    const auto PlayerState = GetSTUPlayerState();
    if (!PlayerState) return 0;
    return PlayerState->GetKillsNum();
}

int32 USTUGameDataWidget::GetCurrentRoundNum() const {
    const auto GameMode = GetSTUGameMode();
    if (!GameMode) return 0;
    return GameMode->GetCurrentRoundNum();
}

int32 USTUGameDataWidget::GetTotalRoundsNum() const {
    const auto GameMode = GetSTUGameMode();
    if (!GameMode) return 0;
    return GameMode->GetGameData().RoundsNum;
}

int32 USTUGameDataWidget::GetRoundSecondsRemaining() const {
    const auto GameMode = GetSTUGameMode();
    if (!GameMode) return 0;
    return GameMode->GetRoundSecondsRemaining();
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const {
    if (!GetWorld()) return nullptr;
    return Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const {
    if (GetOwningPlayer()) return nullptr;
    return Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState);
}
