// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUMenuUserWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUMenuUserWidget : public UUserWidget {
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnStateGame();
};
