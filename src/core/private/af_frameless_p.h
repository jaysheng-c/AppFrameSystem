/**
  ********************************************************************************
  * @file           : frameless_p.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/21
  ********************************************************************************
  */

#ifndef AF_FRAMELESS_P_H
#define AF_FRAMELESS_P_H

#include "af_nocopy.h"
#include <QPointF>
#include "core/global/af_enum.h"

class QMouseEvent;
class QWidget;
class AFWidget;

class AFFramelessPrivate : public AFNoCopy {
public:
    struct MouseAction {
        QPointF pos;
        AF::MouseAction action = AF::NoneAction;
        AF::ResizeEdge edge = AF::NoneEdge;
    };

    explicit AFFramelessPrivate(AFWidget *parent);
    void mousePressEvent(QMouseEvent *event, QWidget *titleWidget);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void setCursorHandle(const QPointF &pos);

protected:
    AF::ResizeEdge resizeEdge(const QPointF &pos) const;
private:
    AFWidget *m_parent;
    MouseAction m_mouseAction;

};


#endif //AF_FRAMELESS_P_H
