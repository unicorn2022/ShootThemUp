// Shoot Them Up Game, All Rights Reserved

#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"
#include "Player/STUPlayerState.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase() {
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay() {
    Super::StartPlay();

    UE_LOG(LogSTUGameModeBase, Display, TEXT("%s"), *GetWorld()->GetGameInstance<USTUGameInstance>()->TestString);

    SpawnBots();
    CreateTeamsInfo();

    CurrentRound = 1;
    StartRound();

    SetMatchState(ESTUMatchState::InProgress);
}

// 为生成的AIController配置Character
UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController) {
    if (InController && InController->IsA<AAIController>())
        return AIPawnClass;
    else
        return Super::GetDefaultPawnClassForController_Implementation(InController);
}

// 击杀事件
void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController) {
    const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    if (KillerPlayerState) KillerPlayerState->AddKill();
    if (VictimPlayerState) VictimPlayerState->AddDeath();

    // 让victim重生
    StartRespawn(VictimController);
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


// 创建队伍信息
void ASTUGameModeBase::CreateTeamsInfo() {
    if (!GetWorld()) return;

    int32 TeamID = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) {
        const auto Controller = It->Get();
        if (!Controller) continue;
        
        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
        PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
        SetPlayerColor(Controller);

        TeamID = TeamID == 1 ? 2 : 1;
    }
}
// 根据TeamID, 决定TeamColor
FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) {
    if (TeamID <= GameData.TeamColors.Num()) {
        return GameData.TeamColors[TeamID - 1];
    } 
    else {
        UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID,
            *GameData.DefaultTeamColor.ToString());
        return GameData.DefaultTeamColor;
    }
}
// 设置玩家颜色
void ASTUGameModeBase::SetPlayerColor(AController* Controller) {
    if (!Controller) return;

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if (!Character) return;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
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
            GameOver();       
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
    SetPlayerColor(Controller);
}


// 输出角色信息到日志
void ASTUGameModeBase::LogPlayerInfo() {
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->LogInfo();
    }
}


// 重新生成角色
void ASTUGameModeBase::StartRespawn(AController* Controller) {
    // 剩余时间不足以重生
    if (RoundCountDown <= GameData.RespawnTime + MinRoundTimeForRespawn) return;

    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
    if (!RespawnComponent) return;

    RespawnComponent->Respawn(GameData.RespawnTime);
}
// 请求重新生成Character
void ASTUGameModeBase::RespawnResqust(AController* Controller) {
    ResetOnePlayer(Controller);
}


// 暂停游戏
bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate) {
    // 先判断能否暂停, 然后再设置游戏状态
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSet) {
        SetMatchState(ESTUMatchState::Pause);
    }
    return PauseSet;
}
// 清除暂停
bool ASTUGameModeBase::ClearPause() {
    const auto PauseCleared = Super::ClearPause();
    if (PauseCleared) {
        SetMatchState(ESTUMatchState::InProgress);
    }
    return PauseCleared;
}


// 游戏结束
void ASTUGameModeBase::GameOver() {
    UE_LOG(LogSTUGameModeBase, Warning, TEXT("=========== Game over =========="));
    LogPlayerInfo();

    for (auto Pawn : TActorRange<APawn>(GetWorld())) {
        if (Pawn) {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetMatchState(ESTUMatchState::GameOver);
}

// 设置游戏状态, 同时广播事件
void ASTUGameModeBase::SetMatchState(ESTUMatchState State) {
    if (MatchState == State) return;

    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}
