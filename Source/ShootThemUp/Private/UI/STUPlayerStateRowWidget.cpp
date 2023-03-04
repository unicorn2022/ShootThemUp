// Shoot Them Up Game, All Rights Reserved

#include "UI/STUPlayerStateRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTUPlayerStateRowWidget::SetPlayerName(const FText& Text) {
    if (!PlayerNameTextBlock) return;
    PlayerNameTextBlock->SetText(Text);
}

void USTUPlayerStateRowWidget::SetKills(const FText& Text) {
    if (!KillsTextBlock) return;
    KillsTextBlock->SetText(Text);
}

void USTUPlayerStateRowWidget::SetDeaths(const FText& Text) {
    if (!DeathsTextBlock) return;
    DeathsTextBlock->SetText(Text);
}

void USTUPlayerStateRowWidget::SetTeam(const FText& Text) {
    if (!TeamTextBlock) return;
    TeamTextBlock->SetText(Text);
}

void USTUPlayerStateRowWidget::SetPlayerIndicatorVisibility(bool Visible) {
    if (!PlayerIndicatorImage) return;
    PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
