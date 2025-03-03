/**
  ********************************************************************************
  * @file           : af_title_widget.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/21
  ********************************************************************************
  */

#include "af_title_widget.h"
#include <QApplication>
#include <QStyle>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStyleOptionFrame>
#include <QResizeEvent>
#include <QPushButton>
#include "global/af_enum.h"

class BtnGenerator {
public:
    explicit BtnGenerator(QWidget *parent = nullptr) : m_parent(parent) {}
    QPushButton *create(const QString &objName, const QString &text, int btnType)
    {
        auto btn = new QPushButton(text, m_parent);
        btn->setObjectName(objName);
        btn->setProperty("type", btnType);
        return btn;
    }
private:
    QWidget *m_parent;
};


AFTitleWidget::AFTitleWidget(QWidget *parent)
    : QWidget(parent), m_iconLabel(new QLabel(this)), m_titleLabel(new QLabel(this)), m_margins(5, 0, 5, 0),
    m_customWidget(new QWidget), m_btnGenerator(new BtnGenerator(this))
{
    this->setObjectName("AFTitleWidget");
    (void) connect(this, &AFTitleWidget::windowIconChanged, this, [this](const QIcon &icon) {
        m_iconLabel->setPixmap(icon.pixmap(QSize(18, 18)));
    });
    (void) connect(this, &AFTitleWidget::windowTitleChanged, this, [this](const QString &title) {
        m_titleLabel->setText(title);
    });

    m_iconLabel->setPixmap(QIcon(":/AF/af.png").pixmap(QSize(18, 18)));
    m_titleLabel->setText("AFTitle");
    auto layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->setContentsMargins(m_margins);
    layout->setSpacing(m_spacing);
    layout->addWidget(m_iconLabel);
    layout->addWidget(m_titleLabel);

    layout->addWidget(m_customWidget);
    auto childLayout = new QHBoxLayout(m_customWidget);
    childLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    childLayout->setSpacing(0);
    childLayout->setContentsMargins({});

    QList<std::tuple<QString, QString, QString, int, bool>> btnPair {
        {"minimize", "—", "最小化", AF::Minimize, true},
        {"maximize", "口", "最大化", AF::Maximize, true},
        {"restore", "口", "最大化", AF::Restore, false},
        {"close", "X", "关闭", AF::Close, true},
    };
    for (const auto & [objName, btnText, tips, type, visible] : btnPair) {
        auto btn = this->addButton(btnText, type);
        btn->setObjectName(objName);
        btn->setToolTip(tips);
        btn->setVisible(visible);
    }
    m_buttons.last()->setObjectName("close");

    layout->setStretch(2, 1);
}

AFTitleWidget::~AFTitleWidget()
{
    qDebug() << Q_FUNC_INFO;
}

QWidget *AFTitleWidget::customWidget() const
{
    auto count = m_customWidget->layout()->count();
    for (int i = 0; i < count; ++i) {
        auto item = m_customWidget->layout()->itemAt(i);
        if (item->widget()) {
            return item->widget();
        }
    }
    return nullptr;
}

void AFTitleWidget::setCustomWidget(QWidget *widget)
{
    auto count = m_customWidget->layout()->count();
    for (int i = 0; i < count; ++i) {
        auto item = m_customWidget->layout()->itemAt(i);
        m_customWidget->layout()->removeItem(item);
    }
    m_customWidget->layout()->addWidget(widget);
}

QPushButton *AFTitleWidget::button(int btnType) const
{
    auto it = std::find_if(m_buttons.cbegin(), m_buttons.cend(), [btnType](const QPushButton *btn) {
        auto var = btn->property("type");
        return var.canConvert<int>() && var.toInt() == btnType;
    });
    if (it != m_buttons.cend()) {
        return *it;
    }
    return nullptr;
}

QPushButton *AFTitleWidget::addButton(const QString &text, int btnType)
{
    auto btn = this->button(btnType);
    if (!btn) {
        btn = m_btnGenerator->create("", text, btnType);
        m_buttons.append(btn);
        this->layout()->addWidget(btn);
        (void) connect(btn, &QPushButton::clicked, this, &AFTitleWidget::onBtnClicked);
    }
    btn->setText(text);
    return btn;
}

void AFTitleWidget::setTitleBtnType(int type) const
{
    auto btnType = AF::BtnType(type);
    if (auto btn = button(AF::Close)) btn->setVisible(btnType.testFlag(AF::Close));
    if (auto btn = button(AF::Minimize)) btn->setVisible(btnType.testFlag(AF::Minimize));
    if (auto btn = button(AF::Maximize)) btn->setVisible(btnType.testFlag(AF::Maximize));
    if (auto btn = button(AF::Restore)) btn->setVisible(btnType.testFlag(AF::Restore));
    if (auto btn = button(AF::FullScreen)) btn->setVisible(btnType.testFlag(AF::FullScreen));
    if (auto btn = button(AF::UserType)) btn->setVisible(btnType.testFlag(AF::UserType));
}

void AFTitleWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    auto size = qMin(this->height(), this->width());
    for (auto btn : m_buttons) {
        btn->setFixedSize({size, size});
    }
}

void AFTitleWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.save();
    auto outerRect = this->rect();
    painter.setPen(Qt::NoPen);
    painter.setBrush(this->palette().base());
    painter.drawRect(outerRect);
    painter.restore();
}

void AFTitleWidget::onBtnClicked()
{
    emit btnClicked(this->sender()->property("type").toInt());
}
