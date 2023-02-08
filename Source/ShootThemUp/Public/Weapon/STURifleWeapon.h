// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon {
    GENERATED_BODY()
public:
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    // �Զ������ʱ����
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;
    // �ӵ������ƫ�ƽǶ�
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float BulletSpread = 1.5f;
    // �����ӵ�
    virtual void MakeShot() override;
    // ��ȡ�ӵ����߼�·��
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
    // �Զ�����Ķ�ʱ��
    FTimerHandle ShotTimerHandle;
};