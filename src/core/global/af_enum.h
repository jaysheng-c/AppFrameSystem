/**
  ********************************************************************************
  * @file           : af_enum.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/21
  ********************************************************************************
  */

#ifndef AF_ENUM_H
#define AF_ENUM_H

namespace AF {
Q_NAMESPACE

enum NoneTypeFlag {
    None = 0x00,
};

Q_DECLARE_FLAGS(NoneType, NoneTypeFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(NoneType)

enum BtnTypeFlag {
    Close = 0x01,
    Minimize = 0x02,
    Maximize = 0x04,
    Restore = 0x08,
    FullScreen = 0x10,

    UserType = 0x1000,
};

Q_DECLARE_FLAGS(BtnType, BtnTypeFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(BtnType)
Q_ENUM_NS(BtnTypeFlag)

enum ResizeEdgeFlag {
    NoneEdge = 0x00,
    Left = 0x01,
    Top = 0x02,
    Right = 0x04,
    Bottom = 0x08,
};
Q_DECLARE_FLAGS(ResizeEdge, ResizeEdgeFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(ResizeEdge)
Q_ENUM_NS(ResizeEdgeFlag)

enum MouseActionFlag {
    NoneAction = 0x00,
    MouseMove,
    MouseResize,
};
Q_DECLARE_FLAGS(MouseAction, MouseActionFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(MouseAction)
Q_ENUM_NS(MouseActionFlag)


}


#endif //AF_ENUM_H
