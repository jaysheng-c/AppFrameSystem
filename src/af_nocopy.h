/**
  ********************************************************************************
  * @file           : af_nocopy.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/31
  ********************************************************************************
  */

#ifndef AF_NOCOPY_H
#define AF_NOCOPY_H

class AFNoCopy {
public:
    AFNoCopy() = default;
    virtual ~AFNoCopy() = default;

    AFNoCopy(const AFNoCopy&) = delete; // 禁止拷贝构造
    AFNoCopy(const AFNoCopy&&) = delete; // 禁止移动构造
    AFNoCopy& operator=(const AFNoCopy&) = delete; // 禁止赋值操作
    AFNoCopy& operator=(const AFNoCopy&&) = delete; // 禁止移动操作
};

#endif //AF_NOCOPY_H
