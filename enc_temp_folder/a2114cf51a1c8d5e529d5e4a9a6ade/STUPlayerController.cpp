// Shoot Them Up Game, All Rights Reserved

#include "Player/STUPlayerController.h"
#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

ASTUPlayerController::ASTUPlayerController() {
    // �����������
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponent");
}

void ASTUPlayerController::BeginPlay() {
    // ���� OnMatchStateChanged �¼�
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
}

// ��ͣ��Ϸ
void ASTUPlayerController::OnPauseGame() {
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->SetPause(this);
}

// ί�У���Ϸ״̬�ı�
void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State) {
    // ��Ϸ������, ֹͣ��ʾ���
    if (State == ESTUMatchState::InProgress) {
        bShowMouseCursor = false;
    } else {
        bShowMouseCursor = true;
    }
}
