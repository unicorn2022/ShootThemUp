// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon() {
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay() {
    Super::BeginPlay();
    check(WeaponMesh);
}

// ����
void ASTUBaseWeapon::Fire() {
    UE_LOG(LogSTUBaseWeapon, Display, TEXT("Fire with Basic Weapon"));
    MakeShot();
}

// �����ӵ�
void ASTUBaseWeapon::MakeShot() {
    if (!GetWorld()) return;

    // ��ȡ������������: �����
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return;
    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller) return;

    // ��ȡ��ҵ�λ�úͳ���
    FVector ViewLocation;
    FRotator ViewRotation;
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    // �ӵ�·��Ϊ: ��ɫ��ǰλ�� -> ��ɫ�泯����
    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const FVector TraceStart = ViewLocation;                // SocketTransform.GetLocation();
    const FVector ShootDirection = ViewRotation.Vector();   // SocketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    // ��ȡ�ӵ�·���ϣ���һ����ײ���Ķ���
    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(GetOwner());   // �����Լ�
    FHitResult HitResult;                               // ��ײ�Ľ��
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

    if (HitResult.bBlockingHit) {
        // �����ӵ���·��: ǹ��λ�� -> ��ײ��
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        // ����ײ������һ����
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        
        // ��ʾ��ײ�����ĸ�������, ����ͨ�������Ϣ�Խ�ɫ��ɲ�ͬ���˺�
        UE_LOG(LogSTUBaseWeapon, Display, TEXT("Fire hit bone: %s"), *HitResult.BoneName.ToString());
    } else {
        // �����ӵ���·��: ǹ��λ�� -> �ӵ�·�����յ�
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}

