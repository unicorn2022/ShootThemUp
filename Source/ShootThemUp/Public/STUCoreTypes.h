#pragma once
#include "STUCoreTypes.generated.h"

/* 武器 */
class ASTUBaseWeapon;
// 委托：弹夹为空
DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);
// 弹药库数据：子弹数量 & 弹夹数量
USTRUCT(BlueprintType)
struct FAmmoData {
    GENERATED_USTRUCT_BODY()

    // 子弹数量
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    // 弹夹数量
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
    int32 Clips;

    // 弹夹是否为无限的
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};
// 武器数据: 类别 & 切换弹夹动画
USTRUCT(BlueprintType)
struct FWeaponData {
    GENERATED_USTRUCT_BODY()

    // 武器的类别
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    // 切换弹夹动画
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};
// 武器UI数据：武器图标 & 瞄准线图标
USTRUCT(BlueprintType)
struct FWeaponUIData {
    GENERATED_USTRUCT_BODY()

    // 武器的图标
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* MainIcon;
    
    // 武器的瞄准线图标
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrossHairIcon;
};

/* 血量 */
// 委托：角色死亡
DECLARE_MULTICAST_DELEGATE(FOnDeath);
// 委托：角色血量变化
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float);


/* 特效 */
class UNiagaraSystem;
class USoundCue;

// 贴花数据：材质 & 大小 & 停留时间 & 淡出动画时间
USTRUCT(BlueprintType)
struct FDecalData {
    GENERATED_USTRUCT_BODY()

    // 贴花材质
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UMaterialInterface* Material;

    // 贴花的大小
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FVector Size = FVector(20.0f);

    // 贴花停留的时间
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float LifeTime = 5.0f;

    // 贴花淡出的时间
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float FadeOutTime = 0.7f;
};
// 击中特效数据：特效 & 贴花 & 击中
USTRUCT(BlueprintType)
struct FImpactData {
    GENERATED_USTRUCT_BODY()

    // Niagara特效
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* NiagaraEffect;

    // 贴花数据
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FDecalData DecalData;

    // 音效：击中
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    USoundCue* Sound;
};

/* 游戏模式 */
// 游戏基础数据
USTRUCT(BlueprintType)
struct FGameData {
    GENERATED_USTRUCT_BODY()

    // 玩家数量
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "100"))
    int32 PlayersNum = 2;

    // 回合数量
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
    int32 RoundsNum = 4;

    // 一回合的时间(s)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
    int32 RoundTime = 10;

    // 默认队伍颜色
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor DefaultTeamColor = FLinearColor::Red;

    // 队伍可选颜色
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<FLinearColor> TeamColors;

    // 重生的时间(s)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "20"))
    int32 RespawnTime = 5;

    // 队伍总数
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "20"))
    int32 TeamCount = 2;
};

/* 游戏菜单 */
// 游戏状态: 等待开始 or 正在进行 or 暂停 or 结束
UENUM(BlueprintType)
enum class ESTUMatchState : uint8 {
    WaitingToStart = 0,
    InProgress,
    Pause,
    GameOver
};
// 委托：游戏状态改变
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, ESTUMatchState);

/* 主菜单 */
USTRUCT(BlueprintType)
struct FLevelData {
    GENERATED_USTRUCT_BODY()

    // 关卡名称
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelName = NAME_None;

    // 显示的关卡名称
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelDisplayName = NAME_None;

    // 关卡缩略图
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    UTexture2D* LevelThumb;
};
// 委托：选择了某个关卡
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);
