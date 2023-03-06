// Shoot Them Up Game, All Rights Reserved

#include "Player/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit) {
    // �����characterÿһ֡����Tick()
    PrimaryActorTick.bCanEverTick = true;

    // �������ɱ����, �������丸���Ϊ�����, ����pawn������ת
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    // ����������, �������丸���Ϊ���ɱ����
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    // ���������������ײ�����, �������丸���Ϊ������
    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ASTUPlayerCharacter::BeginPlay() {
    Super::BeginPlay();

    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
}

// ������
void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

    // WASD���ƽ�ɫ�ƶ�
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);

    // ����������ƶ�
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::AddControllerYawInput);

    // �ո�����ƽ�ɫ��Ծ
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::Jump);

    // ��Shift���ƽ�ɫ��ʼ�ܶ�
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::OnStopRunning);

    // ������������������
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);

    // Tab���л�����
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);

    // R���л�����
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);

    // ����E����׼
    DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Zoom, true);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Released, WeaponComponent, &USTUWeaponComponent::Zoom, false);
}

// WS���ƽ�ɫǰ���ƶ�
void ASTUPlayerCharacter::MoveForward(float Amount) {
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);
}
// AD���ƽ�ɫ�����ƶ�
void ASTUPlayerCharacter::MoveRight(float Amount) {
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
}
// ��Shift���ƽ�ɫ��ʼ�ܶ�
void ASTUPlayerCharacter::OnStartRunning() {
    WantsToRun = true;
}
void ASTUPlayerCharacter::OnStopRunning() {
    WantsToRun = false;
}

// �жϽ�ɫ�Ƿ��ڱ���״̬
bool ASTUPlayerCharacter::IsRunning() const {
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

// ί�У���ɫ����
void ASTUPlayerCharacter::OnDeath() {
    Super::OnDeath();
    // �л�״̬, �Ӷ���pawn�л�Ϊ�۲�����
    if (Controller) {
        Controller->ChangeState(NAME_Spectating);
    }
}

// ί�У�������ɫ��ʼ�ص�
void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    CheckCameraOverlap();
}
// ί�У�������ɫ�����ص�
void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    CheckCameraOverlap();
}
// �������Ƿ񴩹���ɫ����
void ASTUPlayerCharacter::CheckCameraOverlap() {
    // ����ɫ����������Ϊ���ɼ�
    const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);

    // ���������Ϊ���ɼ�
    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);
    for (auto MeshChild : MeshChildren) {
        const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
        if (MeshChildGeometry) MeshChildGeometry->SetOwnerNoSee(HideMesh);
    }
}
