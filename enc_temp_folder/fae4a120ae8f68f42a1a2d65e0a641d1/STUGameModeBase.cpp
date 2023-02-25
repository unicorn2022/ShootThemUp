// Shoot Them Up Game, All Rights Reserved

#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase() {
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}

void ASTUGameModeBase::StartPlay() {
    Super::StartPlay();

    SpawnBots();

    // ��ʼ����һ�غ�
    CurrentRound = 1;
    StartRound();
}

// Ϊ���ɵ�AIController����Character
UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController) {
    if (InController && InController->IsA<AAIController>())
        return AIPawnClass;
    else
        return Super::GetDefaultPawnClassForController_Implementation(InController);
}


// ����AI
void ASTUGameModeBase::SpawnBots() {
    if (!GetWorld()) return;

    for (int32 i = 0; i < GameData.PlayersNum - 1; i++) {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(STUAIController);
    }
}

// ��ʼ�غ�
void ASTUGameModeBase::StartRound() {
    RoundCountDown = GameData.RoundTime;
    // ÿ��һ��, ����RoundCountDown��ֵ
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

// ���¼�ʱ��
void ASTUGameModeBase::GameTimerUpdate() {
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

    RoundCountDown--;
    // ��ǰ�غϽ���
    if (RoundCountDown == 0) {
        // ֹͣ��ʱ��
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
        // �غ���+1
        CurrentRound++;

        // ����ʣ��غ�
        if (CurrentRound <= GameData.RoundsNum) {
            StartRound();
        }
        // �غ��Ѿ�ȫ������
        else {
            UE_LOG(LogSTUGameModeBase, Warning, TEXT("=========== Game over =========="));
        }
    }

}
