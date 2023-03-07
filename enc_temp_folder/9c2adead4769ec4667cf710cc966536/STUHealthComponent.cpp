// Shoot Them Up Game, All Rights Reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"
#include "STUGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay() {
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);

    // 订阅OnTakeAnyDamage事件
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner) {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandler);
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
    }
}

// 委托：角色受到伤害
void USTUHealthComponent::OnTakeAnyDamageHandler(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) {

    UE_LOG(LogSTUHealthComponent, Display, TEXT("On any damage: %f"), Damage);

}
// 委托：角色受到点伤害
void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
    UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser) {
    
    const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
    UE_LOG(LogSTUHealthComponent, Display, TEXT("On point damage: %f, final damage: %f, bone: %s"), Damage, FinalDamage, *BoneName.ToString());
    ApplyDamage(FinalDamage, InstigatedBy);

}
// 委托：角色受到范围伤害
void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
    FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser) {
    
    UE_LOG(LogSTUHealthComponent, Display, TEXT("On radial damage: %f"), Damage);
    ApplyDamage(Damage, InstigatedBy);

}
// 造成伤害
void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy) {
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);

    // 角色受伤时, 停止自动恢复
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    // 角色死亡后, 广播OnDeath委托
    if (IsDead()) {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    // 角色未死亡且可以自动恢复
    else if (AutoHeal) {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }

    // 相机抖动
    PlayCameraShake();
    // 将受到的伤害传递给感官系统
    ReportDamageEvent(Damage, InstigatedBy);
}

// 角色自动恢复
void USTUHealthComponent::HealUpdate() {
    SetHealth(Health + HealModifier);
    // 当角色的生命值最大时, 停止计时器
    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld()) {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

// 设置角色血量
void USTUHealthComponent::SetHealth(float NewHealth) {
    // 保证Health在合理的范围内
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;
    Health = NextHealth;

    // 广播OnHealthChanged委托
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

// 尝试增加血量
bool USTUHealthComponent::TryToAddHealth(float HealthAmount) {
    // 角色已死亡、血量已满，均不可回血
    if (IsDead() || IsHealthFull()) return false;
    
    SetHealth(Health + HealthAmount);
    return true;
}
// 判断角色当前血量是否已满
bool USTUHealthComponent::IsHealthFull() const {
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

// 相机抖动
void USTUHealthComponent::PlayCameraShake() {
    if (IsDead()) return;

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

// 被杀死
void USTUHealthComponent::Killed(AController* KillerController) {
    if (!GetWorld()) return;

    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto VictimController = Player->GetController();
    GameMode->Killed(KillerController, VictimController);
}

// 获取击中某个骨骼需要造成的伤害修正
float USTUHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName) {
    const auto Character = Cast<ACharacter>(DamagedActor);
    if (!Character) return 1.0f;

    const auto Mesh = Character->GetMesh();
    if (!Mesh) return 1.0f;

    const auto BodyInstance = Mesh->GetBodyInstance(BoneName);
    if (!BodyInstance) return 1.0f;

    const auto PhysMaterial = BodyInstance->GetSimplePhysicalMaterial();
    if (!PhysMaterial || !DamageModifiers.Contains(PhysMaterial)) return 1.0f;

    return DamageModifiers[PhysMaterial];
}

// 将受到的伤害传递给感官系统
void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy) {
    if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;

    UAISense_Damage::ReportDamageEvent(GetWorld(),    //
        GetOwner(),                                   //
        InstigatedBy->GetPawn(),                      //
        Damage,                                       //
        InstigatedBy->GetPawn()->GetActorLocation(),  //
        GetOwner()->GetActorLocation());
}
