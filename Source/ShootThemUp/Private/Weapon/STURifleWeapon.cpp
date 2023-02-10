// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTURifleWeapon, All, All);

// ����, ��ͬ�������в�ͬ�Ŀ���ʽ
void ASTURifleWeapon::StartFire() {
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}
// ֹͣ����
void ASTURifleWeapon::StopFire() {
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

// �����ӵ�
void ASTURifleWeapon::MakeShot() {
    // �жϵ�ҩ���Ƿ�Ϊ��
    if (!GetWorld() || IsAmmoEmpty()) {
        StopFire();
        return;
    }

    // ��ȡ�ӵ����߼�·��
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) {
        StopFire();
        return;
    }

    // �����ӵ�����ײ���
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit) {
        // ���ӵ����е���ҽ����˺�
        MakeDamage(HitResult);

        // �����ӵ���·��: ǹ��λ�� -> ��ײ��
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        // ����ײ������һ����
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

        // ��ʾ��ײ�����ĸ�������, ����ͨ�������Ϣ�Խ�ɫ��ɲ�ͬ���˺�
        UE_LOG(LogSTURifleWeapon, Display, TEXT("Fire hit bone: %s"), *HitResult.BoneName.ToString());
    } else {
        // �����ӵ���·��: ǹ��λ�� -> �ӵ�·�����յ�
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }

    // ���ٵ�ҩ��
    DecreaseAmmo();
}

// ��ȡ�ӵ����߼�·��
bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const {
    // ��ȡ��ҵ�λ�úͳ���
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    // �ӵ������Ϊ: ��ɫ��ǰλ��
    TraceStart = ViewLocation;
    // �ӵ��ķ���Ϊ: ��ɫ��ǰ��ǰ�� + һ�������ƫ��
    // ����ɫѪ���ܵ�ʱ, ƫ�ƽǿ��Ա��, ģ���ɫ��׼�����
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    // �ӵ����յ�Ϊ: ��ɫ��ǰλ�����ӵ������˶�һ���ľ���
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

// ���ӵ����е���ҽ����˺�
void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult) {
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor) return;

    DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}