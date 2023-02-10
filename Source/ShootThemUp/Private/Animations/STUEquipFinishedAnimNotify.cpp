// Shoot Them Up Game, All Rights Reserved

#include "Animations/STUEquipFinishedAnimNotify.h"

void USTUEquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
    // 通过骨骼网格体的指针, 区分通知哪一个角色
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}
