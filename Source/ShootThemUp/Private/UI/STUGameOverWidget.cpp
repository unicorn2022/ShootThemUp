// Shoot Them Up Game, All Rights Reserved

#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "UI/STUPlayerStateRowWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "STUUtils.h"
#include "Kismet/GameplayStatics.h"

void USTUGameOverWidget::NativeOnInitialized() {
    Super::NativeOnInitialized();

    // ���� OnMatchStateChanged ί��
    if (GetWorld()) {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode) {
            GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
        }
    }

    // ���� ResetLevelButton �ĵ���¼�
    if (ResetLevelButton) {
        ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel);
    }
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State) {
    if (State == ESTUMatchState::GameOver) {
        UpdatePlayerState();
    }
}

// ���½�ɫ��Ϣ
void USTUGameOverWidget::UpdatePlayerState() {
    if (!GetWorld() || !PlayerStateBox) return;

    // ����ӽڵ�, ��֤��ֱ���еľ�Ϊ������������
    PlayerStateBox->ClearChildren();

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) {
        // ��ȡ��ɫ״̬
        const auto Controller = It->Get();
        if (!Controller) continue;
        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;
        
        // ����UI�ؼ�
        const auto PlayerStateRowWidget = CreateWidget<USTUPlayerStateRowWidget>(GetWorld(), PlayerStateRowWidgetClass);
        if (!PlayerStateRowWidget) continue;

        // �޸�UI�ؼ�����ʾ��Ϣ
        PlayerStateRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStateRowWidget->SetKills(STUUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStateRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerState->GetDeathsNum()));
        PlayerStateRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerStateRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
        PlayerStateRowWidget->SetTeamColor(PlayerState->GetTeamColor());

        // ��UI�ؼ���ӵ���ֱ����
        PlayerStateBox->AddChild(PlayerStateRowWidget);
    }
}

// ί�У����¿�ʼ��Ϸ
void USTUGameOverWidget::OnResetLevel() {
    // Ӳ����: ֱ�����´򿪹ؿ�
    const auto CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
