// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase {
    GENERATED_BODY()
public:
    ASTUGameModeBase();

    // 委托：游戏状态改变
    FOnMatchStateChangedSignature OnMatchStateChanged;

    virtual void StartPlay() override;

    // 为生成的AIController配置Character
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    // 击杀事件
    void Killed(AController* KillerController, AController* VictimController);

    FGameData GetGameData() const { return GameData; }
    int32 GetCurrentRoundNum() const { return CurrentRound; }
    int32 GetRoundSecondsRemaining() const { return RoundCountDown; }

    // 请求重新生成Character
    void RespawnResqust(AController* Controller);

    // 暂停游戏
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    // 清除暂停
    virtual bool ClearPause() override;

protected:
    // AI控制器类
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    // AI角色类
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    // 游戏的基础数据
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

private:
    // 生成AI
    void SpawnBots();
    // 创建队伍信息
    void CreateTeamsInfo();
    // 根据队伍, 设置AI位置
    void SetBotsLocationByTeamID();
    // 根据TeamID, 决定TeamColor
    FLinearColor DetermineColorByTeamID(int32 TeamID);
    // 设置玩家颜色
    void SetPlayerColor(AController* Controller);

private:
    // 当前游戏状态
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
    // 当前回合
    int32 CurrentRound = 1;
    // 当前回合剩余时间
    int32 RoundCountDown = 0;
    // 回合计时器
    FTimerHandle GameRoundTimerHandle;
    // 开始回合
    void StartRound();
    // 更新计时器
    void GameTimerUpdate();

private:
    // 回合开始时，重新生成所有角色
    void ResetPlayers();
    // 重新生成单个角色
    void ResetOnePlayer(AController* Controller);
    // 输出角色信息到日志
    void LogPlayerInfo();

    // 重新生成角色
    void StartRespawn(AController* Controller);

    // 游戏结束
    void GameOver();

    // 设置游戏状态, 同时广播事件
    void SetMatchState(ESTUMatchState State);

    // 停止所有射击
    void StopAllFire();
};
