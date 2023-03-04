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
    // �ı���: �������
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerNameTextBlock;
    // �ı���: ��ɱ��
    UPROPERTY(meta = (BindWidget))
    UTextBlock* KillsTextBlock;
    // �ı���: ������
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeathsTextBlock;
    // �ı���: ��������
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TeamTextBlock;
    // ͼ���: ������ʾ
    UPROPERTY(meta = (BindWidget))
    UImage* PlayerIndicatorImage;
};
