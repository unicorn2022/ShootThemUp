// Shoot Them Up Game, All Rights Reserved

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/ProgressBar.h"
#include "Player/STUPlayerState.h"
#include "STUUtils.h"

float USTUPlayerHUDWidget::GetHealthPercent() const {
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const {
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const {
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

// 玩家是否存活
bool USTUPlayerHUDWidget::IsPlayerAlive() const {
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

// 玩家是否处于观察者模式
bool USTUPlayerHUDWidget::IsPlayerSpectating() const {
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

int32 USTUPlayerHUDWidget::GetKillsNum() const {
    const auto Controller = GetOwningPlayer();
    if (!Controller) return 0;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}

// 格式化子弹数量
FString USTUPlayerHUDWidget::FormatBullets(int32 BulletsNum) const {
    const int32 MaxLen = 3;
    const TCHAR PrefixSymbol = '0';

    auto BulletStr = FString::FromInt(BulletsNum);
    const auto SymbolsNumToAdd = MaxLen - BulletStr.Len();

    if (SymbolsNumToAdd > 0) {
        BulletStr = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletStr);
    }

    return BulletStr;
}

void USTUPlayerHUDWidget::NativeOnInitialized() {
    Super::NativeOnInitialized();

    if (GetOwningPlayer()) {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
        // Initialize会在OnPossess之后调用, 因此需要手动调用一次
        OnNewPawn(GetOwningPlayerPawn());
    }
}

// 玩家血量变化时
void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta) {
    // 血量变化值 < 0, 受到伤害
    if (HealthDelta < 0.0f) {
        OnTakeDamage();
        if (!IsAnimationPlaying(DamageAnimation)) PlayAnimation(DamageAnimation);
    }

    // 更新血量条
    UpdateHealthBar();
}

// 玩家重生时, Pawn会改变
void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn) {
    // 重新订阅HealthComponent的OnHealthChanged事件
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn);
    if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this)) {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }

    // 更新血量条
    UpdateHealthBar();
}

void USTUPlayerHUDWidget::UpdateHealthBar() {
    if (!HealthProgressBar) return;
    const auto FillColor = GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor;
    HealthProgressBar->SetFillColorAndOpacity(FillColor);
}
