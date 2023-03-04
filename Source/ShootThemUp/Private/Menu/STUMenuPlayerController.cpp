// Shoot Them Up Game, All Rights Reserved

#include "Menu/STUMenuPlayerController.h"
#include "STUGameInstance.h"

void ASTUMenuPlayerController::BeginPlay() {
    Super::BeginPlay();

    // 设置输入模式为UI, 并显示光标
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;

    GetWorld()->GetGameInstance<USTUGameInstance>()->TestString = "Menu level say hello!";
}
