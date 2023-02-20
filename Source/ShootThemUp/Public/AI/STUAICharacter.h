// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter {
    GENERATED_BODY()

public:
    ASTUAICharacter(const FObjectInitializer& ObjInit);
};
