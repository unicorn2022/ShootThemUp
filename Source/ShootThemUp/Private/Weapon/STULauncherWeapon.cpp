// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire() {
    MakeShot();
}

void ASTULauncherWeapon::MakeShot() {
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    // �ڳ������ӳٴ���һ����
    auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);
    // �����񵯵Ĳ���
    // ...
    // ����񵯵Ĵ���
    UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}
