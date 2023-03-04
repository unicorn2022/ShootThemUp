// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerStateRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerStateRowWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void SetPlayerName(const FText& Text);
    void SetKills(const FText& Text);
    void SetDeaths(const FText& Text);
    void SetTeam(const FText& Text);
    void SetPlayerIndicatorVisibility(bool Visible);

protected:
    // 文本框: 玩家姓名
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerNameTextBlock;
    // 文本框: 击杀数
    UPROPERTY(meta = (BindWidget))
    UTextBlock* KillsTextBlock;
    // 文本框: 死亡数
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeathsTextBlock;
    // 文本框: 所属队伍
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TeamTextBlock;
    // 图像框: 高亮显示
    UPROPERTY(meta = (BindWidget))
    UImage* PlayerIndicatorImage;
};
