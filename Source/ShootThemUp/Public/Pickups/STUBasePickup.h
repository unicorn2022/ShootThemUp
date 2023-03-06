// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor {
    GENERATED_BODY()

public:
    ASTUBasePickup();

protected:
    // 掉落物的碰撞体
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    USphereComponent* CollisionComponent;

    // 重新生成的间隔时间
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
    float RespawnTime = 5.0f;

    // 是否可以被捡起
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    bool CouldBeTakenTest = true;

    // 音效：拾取
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* PickupSound;

protected:
    virtual void BeginPlay() override;

    // 两个Actor重叠在一起
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
    virtual void Tick(float DeltaTime) override;
    // 可以被捡起
    bool CouldBeTaken() const;

private:
    // 绕Z轴旋转速度
    float RotationYaw = 0.0f;
    // 随机生成一个旋转速度
    void GenerateRotationYaw();

    // 将拾取物给到角色, 用于修改角色属性
    virtual bool GivePickupTo(APawn* PlayerPawn);
    
    // 当前Actor被拾起
    void PickupWasTaken();
    // 重新生成Actor
    void Respawn();
};
