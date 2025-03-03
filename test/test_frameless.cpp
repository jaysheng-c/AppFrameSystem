/**
  ********************************************************************************
  * @file           : test_logger.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/31
  ********************************************************************************
  */


#include <QApplication>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include "src/core/af_widget.h"
#include "src/core/af_title_widget.h"
#include "src/core/global/af_enum.h"

void Flag(AF::BtnType type) {
    qDebug() << type;
    qDebug() << type.testFlag(AF::Close);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleFile(":/QSS/qss/dark.qss");
    if (styleFile.open(QIODevice::ReadOnly)) {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }
    AFWidget w;
    w.show();
    Flag(AF::Close | AF::Maximize);


    auto widget = new AFTitleWidget;
    widget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    widget->setMinimumWidth(0);
    widget->setObjectName("AFTitleWidget2");
    widget->setWindowTitle("title2");
    w.titleWidget()->setCustomWidget(widget);
    qDebug() << w.titleWidget()->minimumSizeHint();

    return QApplication::exec();
}