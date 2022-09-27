#include "../HelpHeaders/Component.h"

uint32_t BaseComponent::lastCompID = 0;

uint32_t BaseComponent::NextPropertyId()
{
    return lastCompID++;
}

template<typename T>
const uint32_t Component<T>::ID = BaseComponent::NextPropertyId();