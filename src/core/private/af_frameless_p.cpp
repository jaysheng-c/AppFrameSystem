/**
  ********************************************************************************
  * @file           : frameless_p.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/21
  ********************************************************************************
  */

#include "af_frameless_p.h"
#include <QMouseEvent>
#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QApplication>
#include <QScreen>
#include <QPushButton>

#include "core/af_widget.h"
#include "core/af_title_widget.h"

namespace {

class Animation {
public:
    QPropertyAnimation *operator()(AFWidget *parent, const QString &property, const QVariant &endValue) const
    {
        auto animation = new QPropertyAnimation(parent, property.toUtf8(), parent);
        animation->setDuration(200);
        animation->setStartValue(parent->property(property.toUtf8()));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->setEndValue(endValue);
        return animation;
    }
};

}

AFFramelessPrivate::AFFramelessPrivate(AFWidget *parent)
    : m_parent(parent)
{

}

void AFFramelessPrivate::mouseDoubleClickEvent(QMouseEvent *event, QWidget *titleWidget)
{
    auto pos = event->position();
    if (event->button() == Qt::LeftButton && titleWidget && titleWidget->geometry().contains(pos.toPoint())) {
        switch (m_parent->windowState()) {
            case Qt::WindowNoState:     showMaximized(); break;
            case Qt::WindowMaximized:   showRestored(); break;
            case Qt::WindowMinimized:
            case Qt::WindowActive:
            case Qt::WindowFullScreen:
                break;
        }
    }
}

void AFFramelessPrivate::mousePressEvent(QMouseEvent *event, QWidget *titleWidget)
{
    if (event->button() != Qt::LeftButton) {
        m_mouseAction = {{}, AF::NoneAction, AF::NoneEdge};
        return;
    }
    auto pos = event->position();
    if (m_mouseAction.edge == AF::NoneEdge) {
        if (titleWidget && titleWidget->geometry().contains(pos.toPoint())) {
            m_mouseAction = {event->globalPosition() - m_parent->pos(), AF::MouseMove, AF::NoneEdge};
        }
    } else if (m_mouseAction.edge != AF::NoneEdge) {
        m_mouseAction.action = AF::MouseResize;
        m_mouseAction.pos = event->globalPosition() - m_parent->pos();
    }
}

void AFFramelessPrivate::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mouseAction.action == AF::NoneAction) {
        return;
    }
    if (m_mouseAction.action == AF::MouseMove) {
        if (m_parent->windowState() == Qt::WindowMaximized) {
            QRect rect(QPoint(event->pos().x() - m_originalRect.width() / 2, (event->pos().y() - 15)),
                       QSize(m_originalRect.width(), m_originalRect.height()));
            auto pressPos = m_mouseAction.pos + m_parent->pos();
            m_mouseAction.pos = pressPos - rect.topLeft();
            m_parent->setGeometry(rect);
            m_parent->showNormal();
            m_parent->titleWidget()->setTitleBtnType(AF::Maximize | AF::Minimize | AF::Close);
        }
        auto pos = event->globalPosition() - m_mouseAction.pos;
        m_parent->move(pos.toPoint());
    } else {
        /**
         * FIXME: AF::Left, AF::Top, AF::Top | AF::Left, AF::Top | AF::Right, AF::Bottom | AF::Left, AF::Bottom | AF::Right 相反方向的边缘抖动，尚未解决
         * */
        auto pos = event->globalPosition() - m_mouseAction.pos;
        auto rect = m_parent->geometry().toRectF();
        switch (m_mouseAction.edge) {
            case AF::None: return;
            case AF::Left:
                rect.setLeft(pos.x());
                if (rect.width() < m_parent->minimumWidth()) {
                    rect.setLeft(rect.right() - m_parent->minimumWidth());
                }
                break;
            case AF::Top:
                rect.setTop(pos.y());
                if (rect.height() < m_parent->minimumHeight()) {
                    rect.setTop(rect.bottom() - m_parent->minimumHeight());
                }
                break;
            case AF::Right:
                rect.setRight(event->globalPosition().toPoint().x());
                if (rect.width() < m_parent->minimumWidth()) {
                    rect.setRight(rect.left() + m_parent->minimumWidth());
                }
                break;
            case AF::Bottom:
                rect.setBottom(event->globalPosition().toPoint().y());
                if (rect.height() < m_parent->minimumHeight()) {
                    rect.setBottom(rect.top() + m_parent->minimumHeight());
                }
                break;
            case AF::Top | AF::Left:
                rect.setTopLeft(pos);
                if (rect.width() < m_parent->minimumWidth()) {
                    rect.setLeft(rect.right() - m_parent->minimumWidth());
                }
                if (rect.height() < m_parent->minimumHeight()) {
                    rect.setTop(rect.bottom() - m_parent->minimumHeight());
                }
                break;
            case AF::Top | AF::Right:
                rect.setTopRight(event->globalPosition());
                if (rect.width() < m_parent->minimumWidth()) {
                    rect.setRight(rect.left() + m_parent->minimumWidth());
                }
                if (rect.height() < m_parent->minimumHeight()) {
                    rect.setTop(rect.bottom() - m_parent->minimumHeight());
                }
                break;
            case AF::Bottom | AF::Left:
                rect.setBottomLeft(event->globalPosition());
                if (rect.width() < m_parent->minimumWidth()) {
                    rect.setLeft(rect.right() - m_parent->minimumWidth());
                }
                if (rect.height() < m_parent->minimumHeight()) {
                    rect.setBottom(rect.top() + m_parent->minimumHeight());
                }
                break;
            case AF::Bottom | AF::Right:
                rect.setBottomRight(event->globalPosition());
                if (rect.width() < m_parent->minimumWidth()) {
                    rect.setRight(rect.left() + m_parent->minimumWidth());
                }
                if (rect.height() < m_parent->minimumHeight()) {
                    rect.setBottom(rect.top() + m_parent->minimumHeight());
                }
                break;
        }
        m_parent->setGeometry({rect.topLeft().toPoint(), rect.size().toSize()});
    }
}

void AFFramelessPrivate::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseAction = {{}, AF::NoneAction, AF::NoneEdge};
}

void AFFramelessPrivate::setCursorHandle(const QPointF &pos)
{
    auto edge = resizeEdge(pos);
    if (edge == AF::Left || edge == AF::Right) {
        m_parent->setCursor(Qt::SizeHorCursor);
    } else if (edge == AF::Top || edge == AF::Bottom) {
        m_parent->setCursor(Qt::SizeVerCursor);
    } else if ((edge == (AF::Top | AF::Left)) || (edge == (AF::Bottom | AF::Right))) {
        m_parent->setCursor(Qt::SizeFDiagCursor);
    } else if ((edge == (AF::Top | AF::Right)) || (edge == (AF::Bottom | AF::Left))) {
        m_parent->setCursor(Qt::SizeBDiagCursor);
    } else {
        m_parent->setCursor(Qt::ArrowCursor);
    }
    m_mouseAction.edge = edge;
}

void AFFramelessPrivate::showMinimized()
{
    if (m_parent->windowState() == Qt::WindowMinimized) {
        return;
    }
    if (m_parent->parentWidget()) {
        // 存在父窗口
    } else {
        m_originalOpacity = m_parent->windowOpacity();
        m_originalRect = m_parent->geometry();
        auto minRect = m_originalRect.adjusted(0, m_parent->height(), -m_parent->width(), 0);

        auto group = new QParallelAnimationGroup;
        Animation animation;
        group->addAnimation(animation(m_parent, "geometry", minRect));
        group->addAnimation(animation(m_parent, "windowOpacity", 0.0));
        (void) QObject::connect(group, &QPropertyAnimation::finished, [this]() {
            m_parent->showMinimized();
        });
        group->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
    }
}

void AFFramelessPrivate::showMaximized()
{
    auto screenGeometry = QApplication::primaryScreen()->availableGeometry();
    m_originalRect = m_parent->geometry();
    auto animation = Animation()(m_parent, "geometry", screenGeometry);
    (void) QObject::connect(animation, &QPropertyAnimation::finished, [this]() {
        m_parent->showMaximized();
        m_parent->titleWidget()->setTitleBtnType(AF::Restore | AF::Minimize | AF::Close);
    });
    animation->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
}

void AFFramelessPrivate::showRestored()
{
    if (m_parent->windowState() == Qt::WindowMaximized) {
        auto animation = Animation()(m_parent, "geometry", m_originalRect);
        (void) QObject::connect(animation, &QPropertyAnimation::finished, [this]() {
            m_parent->showNormal();
            m_parent->titleWidget()->setTitleBtnType(AF::Maximize | AF::Minimize | AF::Close);
        });
        animation->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
    } else {
        auto group = new QParallelAnimationGroup;
        Animation animation;
        group->addAnimation(animation(m_parent, "geometry", m_originalRect));
        group->addAnimation(animation(m_parent, "windowOpacity", m_originalOpacity));
        group->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
    }
}

void AFFramelessPrivate::close()
{
    auto targetRect = m_parent->geometry();
    targetRect = targetRect.adjusted(0, 0, 0, -m_parent->height());
    auto group = new QParallelAnimationGroup;
    Animation animation;
    group->addAnimation(animation(m_parent, "geometry", targetRect));
    group->addAnimation(animation(m_parent, "windowOpacity", 0.0));
    (void) QObject::connect(group, &QPropertyAnimation::finished, [this]() {
        m_parent->close();
    });
    group->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
}

AF::ResizeEdge AFFramelessPrivate::resizeEdge(const QPointF &pos) const
{
    int borderWidth = 5;
    auto rect = m_parent->geometry().toRectF();
    if (pos.x() <= rect.left() + borderWidth) {
        if (pos.y() <= rect.top() + borderWidth) {
            return AF::Top | AF::Left;
        } else if (pos.y() >= rect.bottom() - borderWidth) {
            return AF::Bottom | AF::Left;
        }
        return AF::Left;
    } else if (pos.x() >= rect.right() - borderWidth) {
        if (pos.y() <= rect.top() + borderWidth) {
            return AF::Top | AF::Right;
        } else if (pos.y() >= rect.bottom() - borderWidth) {
            return AF::Bottom | AF::Right;
        }
        return AF::Right;
    } else if (pos.y() <= rect.top() + borderWidth) {
        return AF::Top;
    } else if (pos.y() >= rect.bottom() - borderWidth) {
        return AF::Bottom;
    }
    return AF::NoneEdge;
}
