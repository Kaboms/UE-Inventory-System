#include "Equipment/EquipmentComponentBase.h"
#include "Equipment/EquipmentContainer.h"

void UEquipmentComponentBase::BeginPlay()
{
    Super::BeginPlay();

    EquipmentContainer->Owner = GetOwner();
    EquipmentContainer->Init();
}
