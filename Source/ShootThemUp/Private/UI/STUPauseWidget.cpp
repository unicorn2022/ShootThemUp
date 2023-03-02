// Shoot Them Up Game, All Rights Reserved

#include "UI/STUPauseWidget.h"
#include "Gameframework/GameModeBase.h"
#include "Components/Button.h"

bool USTUPauseWidget::Initialize() {
    const auto InitStatue = Super::Initialize();
    if (ClearPauseButton) {
        ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }
    return InitStatue;
}

// ί�У������ť
void USTUPauseWidget::OnClearPause() {
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}
