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

    virtual void StartPlay() override;

    // 为生成的AIController配置Character
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

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
    // 当前回合
    int32 CurrentRound = 1;
    // 当前回合剩余时间
    int32 RoundCountDown = 0;
    // 回合计时器
    FTimerHandle GameRoundTimerHandle;
    
    // 生成AI
    void SpawnBots();

    // 开始回合
    void StartRound();
    // 更新计时器
    void GameTimerUpdate();
};
