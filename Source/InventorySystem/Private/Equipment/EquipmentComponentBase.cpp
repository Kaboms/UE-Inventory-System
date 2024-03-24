#include "Equipment/EquipmentComponentBase.h"
#include "Equipment/EquipmentContainer.h"

void UEquipmentComponentBase::BeginPlay()
{
    EquipmentContainer->Owner = GetOwner();
    EquipmentContainer->Init();

    Super::BeginPlay();
}
