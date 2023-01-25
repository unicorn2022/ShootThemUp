// Shoot Them Up Game, All Rights Reserved


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const {
    float MaxSpeed = Super::GetMaxSpeed();
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
    
    // 如果角色在跑步, 则最大速度要变大RunModifier倍
    if (Player && Player->IsRunning()) MaxSpeed *= RunModifier;
    return MaxSpeed;
}
