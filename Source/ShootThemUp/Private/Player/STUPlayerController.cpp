// Shoot Them Up Game, All Rights Reserved

#include "Player/STUPlayerController.h"
#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"
#include "STUGameInstance.h"

ASTUPlayerController::ASTUPlayerController() {
    // 创建重生组件
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponent");
}

void ASTUPlayerController::BeginPlay() {
    // 订阅 OnMatchStateChanged 事件
    if (GetWorld()) {
        const auto GameMode = Cast<ASTUGameModeBase>((GetWorld()->GetAuthGameMode()));
        if (GameMode) {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
        }
    }
}

void ASTUPlayerController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);

    OnNewPawn.Broadcast(InPawn);
}

void ASTUPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();
    if (!InputComponent) return;

    InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
    InputComponent->BindAction("Mute", IE_Pressed, this, &ASTUPlayerController::OnMuteSound);
}

// 委托：暂停游戏
void ASTUPlayerController::OnPauseGame() {
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->SetPause(this);
}

// 委托：游戏状态改变
void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State) {
    // 游戏进行中, 停止显示光标
    if (State == ESTUMatchState::InProgress) {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    } else {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}

// 委托：静音
void ASTUPlayerController::OnMuteSound() {
    if (!GetWorld()) return;

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if (!STUGameInstance) return;

    STUGameInstance->ToggleVolume();
}
