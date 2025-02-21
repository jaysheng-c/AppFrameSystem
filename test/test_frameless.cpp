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
#include "src/core/af_dialog.h"
#include "src/core/af_title_widget.h"
#include "src/core/af_main_window.h"

#include <QStatusBar>
#include <QPainter>

class Widget : public QWidget {
public:
    explicit Widget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
//        this->setWindowFlags(Qt::FramelessWindowHint);
//        this->setWindowFlags(Qt::CustomizeWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground, true);
        this->setAttribute(Qt::WA_NoSystemBackground, true);

    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        QPixmap pixmap(size()); // 创建一个与窗口大小相同的缓冲区
        pixmap.fill(Qt::red); // 填充透明色

        QPainter bufferPainter(&pixmap); // 在缓冲区上进行绘制
        // 在这里进行窗口内容的绘制

        painter.drawPixmap(0, 0, pixmap); // 将缓冲区的内容一次性绘制到屏幕上
        qDebug() << this->baseSize() << this->sizeIncrement();
    }

private:
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    AFMainWindow mainWindow;
//    mainWindow.setWindowTitle("af_main_widow");
////    dialog.setWindowTitleWidget(new AFTitleWidget);
////    dialog.setWindowTitle("af_dialog");
//    mainWindow.show();
//
    AFDialog childDlg;
    childDlg.setWindowTitle("af_dialog");
    childDlg.setModal(true);
    childDlg.show();

//    Widget widget;
//    widget.show();

    return QApplication::exec();
}