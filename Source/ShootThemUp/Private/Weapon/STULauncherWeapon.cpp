// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire() {
    MakeShot();
}

void ASTULauncherWeapon::MakeShot() {
    if (!GetWorld()) return;

    // ��ȡ�񵯵��߼�·��
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    // �����񵯵���ײ���
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    // �ж��񵯵����
    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    // �����񵯵��������(��λ����)
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();


    // �񵯵ĳ�ʼλ��
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    // �ڳ������ӳٴ���һ����
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile) {
        // �����񵯵Ĳ���
        Projectile->SetShotDirection(Direction);
        // ����񵯵Ĵ���
        Projectile->FinishSpawning(SpawnTransform);
    }
}
