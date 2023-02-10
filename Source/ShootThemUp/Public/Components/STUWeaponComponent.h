// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

USTRUCT(BlueprintType)
struct FWeaponData {
    GENERATED_USTRUCT_BODY()

    // ���������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    // �л����ж���
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent {
    GENERATED_BODY()

public:
    USTUWeaponComponent();

    // ����
    void StartFire();
    // ֹͣ����
    void StopFire();

    // �л�����
    void NextWeapon();

    // �л�����
    void Reload();

protected:
    // �������������: ������, �л����еĶ���
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;
    // �ֳ������󶨵Ĳ������
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";
    // ���������󶨵Ĳ������
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAmorySocketName = "ArmoySocket";

    // ����װ���Ķ�����̫��
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    // ��ǰ����
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;
    // ��ǰ�����л����еĶ���
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    // ���е�����
    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;


    // ��ǰ����ָ��
    int32 CurrentWeaponIndex = 0;

    // �Ƿ����ڸ�������
    bool EquipAnimInProgress = false;
    // �Ƿ����ڸ�������
    bool ReloadAnimInProgress = false;

private:
    // ��������
    void SpawnWeapons();
    // �������󶨵���ɫ��ĳ�������
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    // װ����������ɫ����
    void EquipWeapon(int32 WeaponIndex);

    // ���Ŷ�����̫��
    void PlayAnimMontage(UAnimMontage* Animation);
    // ��ʼ������֪ͨ
    void InitAnimation();
    // ����֪ͨ�ص����л�����
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    // ����֪ͨ�ص����л�����
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;

    template<typename T>
    T* FindNotifyByClass(UAnimSequenceBase* Animation) {
        if (!Animation) return nullptr;

        const auto NotifyEvents = Animation->Notifies;
        for (auto NotifyEvent : NotifyEvents) {
            auto AnimNotify = Cast<T>(NotifyEvent.Notify);
            if (AnimNotify) return AnimNotify;
        }

        return nullptr;
    }
};
