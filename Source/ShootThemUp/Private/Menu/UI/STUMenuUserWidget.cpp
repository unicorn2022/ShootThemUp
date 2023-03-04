// Shoot Them Up Game, All Rights Reserved

#include "Menu/UI/STUMenuUserWidget.h"
#include "Menu/UI/STULevelItemWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuUserWidget, All, All)

void USTUMenuUserWidget::NativeOnInitialized() {
    Super::NativeOnInitialized();

    if (StartGameButton) {
        StartGameButton->OnClicked.AddDynamic(this, &USTUMenuUserWidget::OnStartGame);
    }

    if (QuitGameButton) {
        QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuUserWidget::OnQuitGame);
    }

    // 初始化关卡选择控件
    InitLevelItems();
}

// 初始化关卡选择控件
void USTUMenuUserWidget::InitLevelItems() {
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance) return;

    checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty!!!"));

    // 清除水平框中的内容
    if (!LevelItemBox) return;
    LevelItemBox->ClearChildren();

    // 针对每一个LevelData, 创建一个控件
    for (auto LevelData : STUGameInstance->GetLevelsData()) {
        const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        // 设置关卡信息
        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuUserWidget::OnLevelSelected);

        // 将控件添加到水平框中
        LevelItemBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }

    // 没有启动关卡：将关卡设置为第0个
    if (STUGameInstance->GetStartupLevel().LevelName.IsNone()) {
        OnLevelSelected(STUGameInstance->GetLevelsData()[0]);
    }
    // 从游戏中返回时, 会有启动关卡, 因此要选中原来的关卡
    else {
        OnLevelSelected(STUGameInstance->GetStartupLevel());
    }
}

// 委托：当前关卡被选中
void USTUMenuUserWidget::OnLevelSelected(const FLevelData& Data) {
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance) return;

    // 将选中的关卡设置为启动关卡
    STUGameInstance->SetStartupLevel(Data);

    for (auto LevelItemWidget : LevelItemWidgets) {
        if (!LevelItemWidget) continue;
        const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
        LevelItemWidget->SetSelected(IsSelected);
    }
}

// 委托：开始游戏
void USTUMenuUserWidget::OnStartGame() {
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance) return;

    UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
}

// 委托：退出游戏
void USTUMenuUserWidget::OnQuitGame() {
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

USTUGameInstance* USTUMenuUserWidget::GetSTUGameInstance() const {
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<USTUGameInstance>();
}
