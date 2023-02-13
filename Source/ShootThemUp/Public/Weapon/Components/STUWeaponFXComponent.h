// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponFXComponent.generated.h"

class UNiagaraSystem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent {
    GENERATED_BODY()

public:
    USTUWeaponFXComponent();

    void PlayImpactFX(const FHitResult& Hit);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* Effect;
};
