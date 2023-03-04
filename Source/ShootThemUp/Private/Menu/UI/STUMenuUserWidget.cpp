// Shoot Them Up Game, All Rights Reserved

#include "Menu/UI/STUMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuUserWidget, All, All)

void USTUMenuUserWidget::NativeOnInitialized() {
    Super::NativeOnInitialized();

    if (StartGameButton) {
        StartGameButton->OnClicked.AddDynamic(this, &USTUMenuUserWidget::OnStartGame);
    }

    if (QuitGameButton) {
        QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuUserWidget::OnQuitGame);
    }
}

void USTUMenuUserWidget::OnStartGame() {
    if (!GetWorld()) return;

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if (!STUGameInstance) return;

    if (STUGameInstance->GetStartupLevelName().IsNone()) {
        UE_LOG(LogSTUMenuUserWidget, Error, TEXT("Startup level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevelName());
}

void USTUMenuUserWidget::OnQuitGame() {
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
