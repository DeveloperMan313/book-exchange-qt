#ifndef BUTTONGROUPS_H
#define BUTTONGROUPS_H

#include <QButtonGroup>

class ButtonGroups {
  public:
    ButtonGroups() = default;

    QButtonGroup login;
    QButtonGroup adminMenu;
};

extern ButtonGroups *buttonGroups;

#endif // BUTTONGROUPS_H
