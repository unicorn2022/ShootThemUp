// Shoot Them Up Game, All Rights Reserved

#include "Menu/UI/STULevelItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTULevelItemWidget::NativeOnInitialized() {
    Super::NativeOnInitialized();

    if (LevelSelectButton) {
        LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
    }
}

void USTULevelItemWidget::OnLevelItemClicked() {
    OnLevelSelected.Broadcast(LevelData);
}

void USTULevelItemWidget::SetLevelData(const FLevelData& Data) {
    LevelData = Data;

    if (LevelNameTextBlock) {
        LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
    }

    if (LevelImage){
        LevelImage->SetBrushFromTexture(Data.LevelThumb);    
    } 
}

void USTULevelItemWidget::SetSelected(bool IsSelected) {
    if (FrameImage) {
        FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}