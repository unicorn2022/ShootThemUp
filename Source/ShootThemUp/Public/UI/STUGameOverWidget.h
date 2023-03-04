// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUGameOverWidget.generated.h"

class UVerticalBox;

UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public UUserWidget {
    GENERATED_BODY()

public:
    virtual bool Initialize() override;

protected:
    // 显示角色信息的垂直框
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStateBox;

    // 角色信息单行显示UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStateRowWidgetClass;

private:
    void OnMatchStateChanged(ESTUMatchState State);
    // 更新角色信息
    void UpdatePlayerState();
};
