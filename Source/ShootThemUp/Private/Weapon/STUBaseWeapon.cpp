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
    
    // ��ȡ�ӵ����߼�·��
    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    // �����ӵ�����ײ���
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);
    
    if (HitResult.bBlockingHit) {
        // �����ӵ���·��: ǹ��λ�� -> ��ײ��
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        // ����ײ������һ����
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        
        // ��ʾ��ײ�����ĸ�������, ����ͨ�������Ϣ�Խ�ɫ��ɲ�ͬ���˺�
        UE_LOG(LogSTUBaseWeapon, Display, TEXT("Fire hit bone: %s"), *HitResult.BoneName.ToString());
    } else {
        // �����ӵ���·��: ǹ��λ�� -> �ӵ�·�����յ�
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}

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

    // �ӵ�·��Ϊ: ��ɫ��ǰλ�� -> ��ɫ�泯����
    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
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