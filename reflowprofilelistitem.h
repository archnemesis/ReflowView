#ifndef REFLOWPROFILELISTITEM_H
#define REFLOWPROFILELISTITEM_H

#include <QListWidgetItem>
#include "reflowprofile.h"

class ReflowProfileListItem : public QListWidgetItem
{
public:
    void setReflowProfile(ReflowProfile &profile);
    ReflowProfile profile();

protected:
    ReflowProfile m_profile;
};

#endif // REFLOWPROFILELISTITEM_H
