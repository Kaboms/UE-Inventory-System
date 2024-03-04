#include "Equipment/EquipmentContainer.h"
#include "Equipment/EquipSlotBase.h"

UEquipmentContainer::UEquipmentContainer()
{
    for (auto& SlotPair : Slots)
    {
        SlotPair.Value->SlotTag = SlotPair.Key;
    }
}
