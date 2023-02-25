// Shoot Them Up Game, All Rights Reserved

#include "Player/STUPlayerController.h"
#include "Components/STURespawnComponent.h"

ASTUPlayerController::ASTUPlayerController() {
    // 创建重生组件
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponent");
}
