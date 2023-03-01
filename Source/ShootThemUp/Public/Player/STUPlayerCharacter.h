// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter {
    GENERATED_BODY()

public:
    // ����CharacterMovementComponent�����Ĭ�����, ���������Ҫͨ��������ʽָ��
    ASTUPlayerCharacter(const FObjectInitializer& ObjInit);

protected:
    // ���������ĵ��ɱ�
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;
    // ��������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;
    // ����������������ײ��
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* CameraCollisionComponent;

    // ί�У���ɫ����
    virtual void OnDeath() override;
    virtual void BeginPlay() override;

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // �жϽ�ɫ�Ƿ��ڱ���״̬
    virtual bool IsRunning() const override;

private:
    // WS���ƽ�ɫǰ���ƶ�
    bool IsMovingForward = false;
    void MoveForward(float Amount);
    // AD���ƽ�ɫ�����ƶ�
    void MoveRight(float Amount);

    // ��Shift���ƽ�ɫ��ʼ�ܶ�
    bool WantsToRun = false;  // ����Shiftֻ�ܱ�ʾ��Ҫ�ܲ�, ֻ�е�������Wʱ, ���ܿ�ʼ�ܲ�
    void OnStartRunning();
    void OnStopRunning();

    // ί�У�������ɫ��ʼ�ص�
    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    // ί�У�������ɫ�����ص�
    UFUNCTION()
    void OnCameraCollisionEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    // �������Ƿ񴩹���ɫ����
    void CheckCameraOverlap();
};
