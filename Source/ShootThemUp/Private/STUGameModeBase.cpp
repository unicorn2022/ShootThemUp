// Shoot Them Up Game, All Rights Reserved

#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"

ASTUGameModeBase::ASTUGameModeBase() {
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}

void ASTUGameModeBase::StartPlay() {
    Super::StartPlay();

    SpawnBots();
}

// 为生成的AIController配置Character
UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController) {
    if (InController && InController->IsA<AAIController>())
        return AIPawnClass;
    else
        return Super::GetDefaultPawnClassForController_Implementation(InController);
}


// 生成AI
void ASTUGameModeBase::SpawnBots() {
    if (!GetWorld()) return;

    for (int32 i = 0; i < GameData.PlayersNum - 1; i++) {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(STUAIController);
    }
}