// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

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

protected:
    // ���������
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<ASTUBaseWeapon>> WeaponClasses;
    // �ֳ������󶨵Ĳ������
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";
    // ���������󶨵Ĳ������
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAmorySocketName = "AmorySocket";

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    // ��ǰ����
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    // ���е�����
    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    // ��ǰ����ָ��
    int32 CurrentWeaponIndex = 0;

    // ��������
    void SpawnWeapons();
    // �������󶨵���ɫ��ĳ�������
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    // װ����������ɫ����
    void EquipWeapon(int32 WeaponIndex);
};
