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

    // 订阅 OnMatchStateChanged 委托
    if (GetWorld()) {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode) {
            GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
        }
    }

    // 订阅 ResetLevelButton 的点击事件
    if (ResetLevelButton) {
        ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel);
    }
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State) {
    if (State == ESTUMatchState::GameOver) {
        UpdatePlayerState();
    }
}

// 更新角色信息
void USTUGameOverWidget::UpdatePlayerState() {
    if (!GetWorld() || !PlayerStateBox) return;

    // 清空子节点, 保证垂直框中的均为本函数创建的
    PlayerStateBox->ClearChildren();

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) {
        // 获取角色状态
        const auto Controller = It->Get();
        if (!Controller) continue;
        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;
        
        // 创建UI控件
        const auto PlayerStateRowWidget = CreateWidget<USTUPlayerStateRowWidget>(GetWorld(), PlayerStateRowWidgetClass);
        if (!PlayerStateRowWidget) continue;

        // 修改UI控件的显示信息
        PlayerStateRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStateRowWidget->SetKills(STUUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStateRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerState->GetDeathsNum()));
        PlayerStateRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerStateRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
        PlayerStateRowWidget->SetTeamColor(PlayerState->GetTeamColor());

        // 将UI控件添加到垂直框中
        PlayerStateBox->AddChild(PlayerStateRowWidget);
    }
}

// 委托：重新开始游戏
void USTUGameOverWidget::OnResetLevel() {
    // 硬重置: 直接重新打开关卡
    const auto CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
