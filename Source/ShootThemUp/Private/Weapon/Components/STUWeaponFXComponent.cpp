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

    // ����Niagaraϵͳ
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
        ImpactData.NiagaraEffect,      // Niagaraϵͳ
        Hit.ImpactPoint,               // λ��
        Hit.ImpactNormal.Rotation());  // ��ת

    // ��������
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
        ImpactData.DecalData.Material,  // ��������
        ImpactData.DecalData.Size,      // ������С
        Hit.ImpactPoint,                // λ��
        Hit.ImpactNormal.Rotation());   // ��ת
    // һ��ʱ��󵭳�
    if (DecalComponent) {
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
    }

    // ������Ч
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.Sound, Hit.ImpactPoint);
}
