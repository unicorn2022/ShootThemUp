// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUCoreTypes.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController {
    GENERATED_BODY()

public:
    ASTUPlayerController();

protected:
    // 重生组件
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTURespawnComponent* RespawnComponent;

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void SetupInputComponent() override;

private:
    // 暂停游戏
    void OnPauseGame();
    // 委托：游戏状态改变
    void OnMatchStateChanged(ESTUMatchState State);
};
