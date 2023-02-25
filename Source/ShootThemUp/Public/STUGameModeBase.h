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
    // 生成AI
    void SpawnBots();
};
