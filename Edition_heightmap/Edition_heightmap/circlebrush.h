#pragma once
#ifndef circlebrush_h
#define circlebrush_h

#include <Windows.h>
#include <opencv2/core/types.hpp>

//Class CircleBrush : un disque avec un effet fondu aux frontieres
class CircleBrush
{
public:
    CircleBrush();
    CircleBrush(int radiusInt, int radiusExt, int color);
    void setRadiusInt(int newValue);
    void setRadiusExt(int newValue);
    void setColor(int newColor);
    int getRadiusInt() const;
    int getRadiusExt() const;
    void paint(int x, int y, cv::Mat& image) const;


private:
    int m_radiusInt;
    int m_radiusExt;
    int m_color;
};


#endif