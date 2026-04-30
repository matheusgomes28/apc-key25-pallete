#ifndef BUTTON_STATE_H
#define BUTTON_STATE_H

#include <QObject>

namespace apc25 {
    Q_NAMESPACE;

    enum class ButtonState : unsigned char {
        Off,
        Green,
        GreenBlink,
        Red,
        RedBlink,
        Yellow,
        YellowBlink,
    };

    Q_ENUM_NS(ButtonState);
} // namespace

#endif // BUTTON_STATE_H
