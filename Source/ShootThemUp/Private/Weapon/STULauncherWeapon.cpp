// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire() {
    MakeShot();
}

void ASTULauncherWeapon::MakeShot() {
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    // 在场景中延迟创建一个榴弹
    auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);
    // 设置榴弹的参数
    // ...
    // 完成榴弹的创建
    UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}
