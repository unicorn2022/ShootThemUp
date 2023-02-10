#pragma once
#include "STUCoreTypes.generated.h"

/* Weapon */
class ASTUBaseWeapon;

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

USTRUCT(BlueprintType)
struct FAmmoData {
    GENERATED_USTRUCT_BODY()

    // �ӵ�����
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    // ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
    int32 Clips;

    // �����Ƿ�Ϊ���޵�
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

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

/* Health */
DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);
