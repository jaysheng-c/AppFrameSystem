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
#include <QRect>
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
    void mouseDoubleClickEvent(QMouseEvent *event, QWidget *titleWidget);
    void mousePressEvent(QMouseEvent *event, QWidget *titleWidget);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void setCursorHandle(const QPointF &pos);
    void showMinimized();
    void showMaximized();
    void showRestored();

protected:
    [[nodiscard]] AF::ResizeEdge resizeEdge(const QPointF &pos) const;
private:
    AFWidget *m_parent;
    MouseAction m_mouseAction;

    QRect m_originalRect;
    qreal m_originalOpacity = 1.0;
};


#endif //AF_FRAMELESS_P_H
