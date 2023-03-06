// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public USTUBaseWidget {
    GENERATED_BODY()

protected:
    // ��ֱ����ʾ��ɫ��Ϣ
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStateBox;

    // Widget����ɫ��Ϣ������ʾUI
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStateRowWidgetClass;

    // ��ť�����¿�ʼ��Ϸ
    UPROPERTY(meta = (BindWidget))
    UButton* ResetLevelButton;

    // Initialize()ʱ���õĺ���
    virtual void NativeOnInitialized() override;

private:
    void OnMatchStateChanged(ESTUMatchState State);
    // ���½�ɫ��Ϣ
    void UpdatePlayerState();

    // ί�У����¿�ʼ��Ϸ
    UFUNCTION()
    void OnResetLevel();
};
