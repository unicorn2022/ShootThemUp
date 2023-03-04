// Shoot Them Up Game, All Rights Reserved

#include "UI/STUPauseWidget.h"
#include "Gameframework/GameModeBase.h"
#include "Components/Button.h"

void USTUPauseWidget::NativeOnInitialized() {
    Super::NativeOnInitialized();
    if (ClearPauseButton) {
        ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }
}

// ί�У������ť
void USTUPauseWidget::OnClearPause() {
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}
