/**
  ********************************************************************************
  * @file           : af_title_widget.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/21
  ********************************************************************************
  */

#ifndef AF_TITLE_WIDGET_H
#define AF_TITLE_WIDGET_H

#include <QWidget>

class QLabel;
class QPushButton;
class BtnGenerator;


class AFTitleWidget : public QWidget  {
Q_OBJECT
    Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
    Q_PROPERTY(QWidget* customWidget READ customWidget WRITE setCustomWidget NOTIFY customWidgetChanged)
signals:
    void customWidgetChanged(QWidget *widget);
    void btnClicked(int btnType);
public:
    explicit AFTitleWidget(QWidget *parent = nullptr);
    ~AFTitleWidget() override;

    QMargins margins() const { return m_margins; }
    int spacing() const { return m_spacing; }
    QWidget *customWidget() const;
    void setMargins(const QMargins &margins) { m_margins = margins; }
    void setSpacing(int spacing) { m_spacing = spacing; }
    void setCustomWidget(QWidget *widget);

    // 标题栏按键
    QPushButton *button(int btnType) const;
    QPushButton *addButton(const QString &text, int btnType);
    void setTitleBtnType(int type) const;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void onBtnClicked();

private:
    QMargins m_margins;
    int m_spacing = 5;

    QLabel *m_iconLabel;
    QLabel *m_titleLabel;

    QList<QPushButton*> m_buttons;
    QWidget *m_customWidget;

    QScopedPointer<BtnGenerator> m_btnGenerator;
};


#endif //AF_TITLE_WIDGET_H
