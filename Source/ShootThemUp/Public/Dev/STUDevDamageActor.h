// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamageActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUDevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUDevDamageActor();

	// 添加场景组件, 使其具有变换效果, 并且在场景中能够移动
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* SceneComponent;

	// 绘制该Actor的影响范围的参数
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Radius = 300.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor SphereColor = FColor::Red;

	// 该Actor造成伤害的参数
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 10.0f;
    // DoFullDamage: 为true时, 对整个球体内的角色造成等量伤害; 为false时, 离球心越远, 角色收到的伤害越少
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool DoFullDamage = false;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
