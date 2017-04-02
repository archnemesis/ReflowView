#include "reflowprofilelistitem.h"

void ReflowProfileListItem::setReflowProfile(ReflowProfile &profile)
{
    m_profile = profile;
}

ReflowProfile ReflowProfileListItem::profile()
{
    return m_profile;
}
