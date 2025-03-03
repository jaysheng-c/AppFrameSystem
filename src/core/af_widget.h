/**
  ********************************************************************************
  * @file           : af_widget.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/21
  ********************************************************************************
  */

#ifndef AF_WIDGET_H
#define AF_WIDGET_H

#include <QWidget>
#include "../af_nocopy.h"

class AFTitleWidget;
class AFStatusWidget;
class AFFramelessPrivate;

class AFWidget : public QWidget, public AFNoCopy {
Q_OBJECT
friend class AFFramelessPrivate;
public:
    explicit AFWidget(QWidget *parent = nullptr);

    [[nodiscard]] AFTitleWidget *titleWidget() const { return m_titleWidget; }
    [[nodiscard]] AFStatusWidget *statusWidget() const { return m_statusWidget; }
    [[nodiscard]] QWidget *customWidget() const { return m_customWidget; }
    void setTitleWidget(AFTitleWidget *widget);
    void setStatusWidget(AFStatusWidget *widget);
    void setCustomWidget(QWidget *widget);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void changeEvent(QEvent *event) override;

public:
    void onBtnClicked(int type);

private:
    AFTitleWidget *m_titleWidget;
    AFStatusWidget *m_statusWidget;
    QWidget *m_customWidget;


    AFFramelessPrivate *m_d;
};


#endif //AF_WIDGET_H
