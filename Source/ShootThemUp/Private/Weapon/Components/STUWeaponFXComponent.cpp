// Shoot Them Up Game, All Rights Reserved

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundCue.h"

USTUWeaponFXComponent::USTUWeaponFXComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit) {
    auto ImpactData = DefaultImpactData;

    if (Hit.PhysMaterial.IsValid()) {
        const auto PhysMat = Hit.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMat)) {
            ImpactData = ImpactDataMap[PhysMat];
        }
    }

    // 生成Niagara系统
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
        ImpactData.NiagaraEffect,      // Niagara系统
        Hit.ImpactPoint,               // 位置
        Hit.ImpactNormal.Rotation());  // 旋转

    // 生成贴花
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
        ImpactData.DecalData.Material,  // 贴花材质
        ImpactData.DecalData.Size,      // 贴花大小
        Hit.ImpactPoint,                // 位置
        Hit.ImpactNormal.Rotation());   // 旋转
    // 一段时间后淡出
    if (DecalComponent) {
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
    }

    // 播放音效
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.Sound, Hit.ImpactPoint);
}
