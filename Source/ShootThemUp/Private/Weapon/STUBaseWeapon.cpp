// Shoot Them Up Game, All Rights Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon() {
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay() {
    Super::BeginPlay();
    
    check(WeaponMesh);
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets can't be <= 0"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips can't be <= 0"));
    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire() {}
void ASTUBaseWeapon::StopFire() {}
void ASTUBaseWeapon::MakeShot() {}

// ��ȡ��ҿ�����
APlayerController* ASTUBaseWeapon::GetPlayerController() const {
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;
    return Player->GetController<APlayerController>();
}

// ��ȡ��ҵ�λ�úͳ���
bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const {
    const auto Controller = GetPlayerController();
    if (!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

// ��ȡǹ�ڵ�λ��
FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const {
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

// ��ȡ�ӵ����߼�·��
bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const {
    // ��ȡ��ҵ�λ�úͳ���
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    // �ӵ������Ϊ: ��ɫ��ǰλ��
    TraceStart = ViewLocation;
    // �ӵ��ķ���Ϊ: ��ɫ��ǰ��ǰ��
    const FVector ShootDirection = ViewRotation.Vector();
    // �ӵ����յ�Ϊ: ��ɫ��ǰλ�����ӵ������˶�һ���ľ���
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

// ִ����ײ�߼�
void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const {
    if (!GetWorld()) return;
    
    // �����Լ�
    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(GetOwner());  
    
    // ��ȡ�ӵ�·���ϣ���һ����ײ���Ķ��󣬴洢��HitResult��
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
}

// ÿ�η��������ӵ�
void ASTUBaseWeapon::DecreaseAmmo() {
    CurrentAmmo.Bullets--;
    LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty()) ChangeClip();
}
// �жϵ�ҩ���Ƿ�Ϊ��
bool ASTUBaseWeapon::IsAmmoEmpty() const {
    return !CurrentAmmo.Infinite && CurrentAmmo.Bullets == 0 && CurrentAmmo.Clips == 0;
}
// �жϵ����Ƿ�Ϊ��
bool ASTUBaseWeapon::IsClipEmpty() const {
    return CurrentAmmo.Bullets == 0;
}
// �л�����
void ASTUBaseWeapon::ChangeClip() {
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    if (!CurrentAmmo.Infinite) CurrentAmmo.Clips--;
    UE_LOG(LogSTUBaseWeapon, Display, TEXT("------ Change Clip ------"));
}
// ����ҩ����Ϣ��ʾ������̨
void ASTUBaseWeapon::LogAmmo() const {
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogSTUBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}