// Shoot Them Up Game, All Rights Reserved

#include "Animations/STUEquipFinishedAnimNotify.h"

void USTUEquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
    // ͨ�������������ָ��, ����֪ͨ��һ����ɫ
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}
