// Shoot Them Up Game, All Rights Reserved

#include "Dev/STUDevDamageActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUDevDamageActor, All, All);

ASTUDevDamageActor::ASTUDevDamageActor() {
    // 允许每一帧调用Tick()
    PrimaryActorTick.bCanEverTick = true;

    // 添加场景组件, 并将其设置为根组件
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);
}

void ASTUDevDamageActor::BeginPlay() {
    Super::BeginPlay();
}

void ASTUDevDamageActor::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    // 绘制一个球体, 显示当前Actor的影响范围
    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);

    // 造成球状范围伤害
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, nullptr, {}, this, nullptr, DoFullDamage);
}
