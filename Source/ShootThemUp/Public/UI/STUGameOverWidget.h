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
    // ��ʾ��ɫ��Ϣ�Ĵ�ֱ��
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStateBox;

    // ��ɫ��Ϣ������ʾUI
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStateRowWidgetClass;

private:
    void OnMatchStateChanged(ESTUMatchState State);
    // ���½�ɫ��Ϣ
    void UpdatePlayerState();
};
