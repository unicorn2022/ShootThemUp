// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire() {
    MakeShot();
}

void ASTULauncherWeapon::MakeShot() {
    if (!GetWorld()) return;

    // 获取榴弹的逻辑路径
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    // 计算榴弹的碰撞结果
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    // 判断榴弹的落点
    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    // 计算榴弹的射击方向(单位向量)
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();


    // 榴弹的初始位置
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    // 在场景中延迟创建一个榴弹
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile) {
        // 设置榴弹的参数
        Projectile->SetShotDirection(Direction);
        // 完成榴弹的创建
        Projectile->FinishSpawning(SpawnTransform);
    }
}
