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

    // ��ʼ���ؿ�ѡ��ؼ�
    InitLevelItems();
}

// ��ʼ���ؿ�ѡ��ؼ�
void USTUMenuUserWidget::InitLevelItems() {
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance) return;

    checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty!!!"));

    // ���ˮƽ���е�����
    if (!LevelItemBox) return;
    LevelItemBox->ClearChildren();

    // ���ÿһ��LevelData, ����һ���ؼ�
    for (auto LevelData : STUGameInstance->GetLevelsData()) {
        const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        // ���ùؿ���Ϣ
        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuUserWidget::OnLevelSelected);

        // ���ؼ���ӵ�ˮƽ����
        LevelItemBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }

    // û�������ؿ������ؿ�����Ϊ��0��
    if (STUGameInstance->GetStartupLevel().LevelName.IsNone()) {
        OnLevelSelected(STUGameInstance->GetLevelsData()[0]);
    }
    // ����Ϸ�з���ʱ, ���������ؿ�, ���Ҫѡ��ԭ���Ĺؿ�
    else {
        OnLevelSelected(STUGameInstance->GetStartupLevel());
    }
}

// ί�У���ǰ�ؿ���ѡ��
void USTUMenuUserWidget::OnLevelSelected(const FLevelData& Data) {
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance) return;

    // ��ѡ�еĹؿ�����Ϊ�����ؿ�
    STUGameInstance->SetStartupLevel(Data);

    for (auto LevelItemWidget : LevelItemWidgets) {
        if (!LevelItemWidget) continue;
        const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
        LevelItemWidget->SetSelected(IsSelected);
    }
}

// ί�У���ʼ��Ϸ
void USTUMenuUserWidget::OnStartGame() {
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance) return;

    UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
}

// ί�У��˳���Ϸ
void USTUMenuUserWidget::OnQuitGame() {
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

USTUGameInstance* USTUMenuUserWidget::GetSTUGameInstance() const {
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<USTUGameInstance>();
}
