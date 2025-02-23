/**
  ********************************************************************************
  * @file           : af_widget.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/21
  ********************************************************************************
  */

#include "af_widget.h"

#include <QVBoxLayout>
#include <QMouseEvent>

#include "private/af_frameless_p.h"
#include "af_status_widget.h"
#include "af_title_widget.h"
#include "log/af_logger.h"

AFWidget::AFWidget(QWidget *parent)
    : QWidget(parent), m_titleWidget(new AFTitleWidget(this)), m_statusWidget(new AFStatusWidget(this)),
    m_customWidget(new QWidget(this)), m_d(new AFFramelessPrivate(this))
{
    this->setAttribute(Qt::WA_Hover, true);
    this->setMouseTracking(true);
    this->installEventFilter(this);

    this->resize(600, 400);
    this->setLayout(new QVBoxLayout);
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);
    this->layout()->addWidget(m_titleWidget);
    this->layout()->addWidget(m_customWidget);
    this->layout()->addWidget(m_statusWidget);

    m_titleWidget->setFixedHeight(30);
//    m_titleWidget->setStyleSheet("QWidget{background-color: rgb(255, 0, 0);}");
    m_statusWidget->setFixedHeight(30);
    m_statusWidget->setStyleSheet("QWidget{background-color: rgb(0, 255, 0);}");

    this->setWindowFlags(Qt::FramelessWindowHint | this->windowFlags());
//    AF_Debug(AFWidget) << this->windowFlags();

    connect(m_titleWidget, &AFTitleWidget::btnClicked, this, &AFWidget::onBtnClicked);
}

void AFWidget::setTitleWidget(AFTitleWidget *widget)
{
    if (m_titleWidget == widget) {
        return;
    }
    delete m_titleWidget;
    m_titleWidget = widget;
    if (m_titleWidget) {
        m_titleWidget->setParent(this);
    }
}

void AFWidget::setStatusWidget(AFStatusWidget *widget)
{
    if (m_statusWidget == widget) {
        return;
    }
    delete m_titleWidget;
    m_statusWidget = widget;
    if (m_statusWidget) {
        m_statusWidget->setParent(this);
    }
}

void AFWidget::setCustomWidget(QWidget *widget)
{
    if (m_customWidget == widget) {
        return;
    }
    delete m_customWidget;
    m_customWidget = widget;
    if (m_customWidget) {
        m_customWidget->setParent(this);
    }
}

bool AFWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::HoverMove) {
        auto e = dynamic_cast<QHoverEvent *>(event);
        m_d->setCursorHandle(e->globalPosition());
    }
    return QObject::eventFilter(obj, event);
}

void AFWidget::mousePressEvent(QMouseEvent *event)
{
    m_d->mousePressEvent(event, m_titleWidget);
    QWidget::mousePressEvent(event);
}

void AFWidget::mouseMoveEvent(QMouseEvent *event)
{
    AF_Debug(AFWidget) << Q_FUNC_INFO;
    m_d->mouseMoveEvent(event);
    QWidget::mouseMoveEvent(event);
}

void AFWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_d->mouseReleaseEvent(event);
    QWidget::mouseReleaseEvent(event);
}

void AFWidget::onBtnClicked(int type)
{
    switch (type) {
        case AF::Close:     this->close(); break;
        case AF::Minimize:  this->showMinimized(); break;
        case AF::Maximize:  this->showMaximized(); break;
        case AF::Restore:   this->showNormal(); break;
        case AF::UserType:
        default:
            break;
    }

}
