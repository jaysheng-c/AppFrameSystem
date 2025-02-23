/**
  ********************************************************************************
  * @file           : af_status_widget.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/21
  ********************************************************************************
  */

#ifndef AF_STATUS_WIDGET_H
#define AF_STATUS_WIDGET_H

#include <QWidget>

class AFStatusWidget : public QWidget {
    Q_OBJECT
public:
    explicit AFStatusWidget(QWidget *parent = nullptr) : QWidget(parent) {}
};


#endif //AF_STATUS_WIDGET_H
