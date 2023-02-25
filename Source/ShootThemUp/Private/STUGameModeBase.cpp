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

    // 初始化第一回合
    CurrentRound = 1;
    StartRound();
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

// 开始回合
void ASTUGameModeBase::StartRound() {
    RoundCountDown = GameData.RoundTime;
    // 每秒一次, 减少RoundCountDown的值
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

// 更新计时器
void ASTUGameModeBase::GameTimerUpdate() {
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

    RoundCountDown--;
    // 也可以使用如下方案, 但是此时RoundCountDown就是float了
    // const auto TimerRate = GetWorldTimerManager().GetTimerRate(GameRoundTimerHandle);
    // RoundCountDown -= TimerRate;

    // 当前回合结束
    if (RoundCountDown == 0) {
        // 停止计时器
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
        // 回合数+1
        CurrentRound++;

        // 仍有剩余回合
        if (CurrentRound <= GameData.RoundsNum) {
            ResetPlayers();
            StartRound();
        }
        // 回合已经全部结束
        else {
            UE_LOG(LogSTUGameModeBase, Warning, TEXT("=========== Game over =========="));
        }
    }

}

// 回合开始时，重新生成所有角色
void ASTUGameModeBase::ResetPlayers() {
    if (!GetWorld()) return;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) {
        ResetOnePlayer(It->Get());
    }
}

// 重新生成单个角色
void ASTUGameModeBase::ResetOnePlayer(AController* Controller) {
    // 当Controller已经控制Character时, RestartPlayer时, SpawnRotation会直接使用当前控制的角色的Rotation
    // 因此需要将当前控制的角色Reset()一下, 实现重开的效果
    if (Controller && Controller->GetPawn()) {
        Controller->GetPawn()->Reset();
    }

    RestartPlayer(Controller);
}
