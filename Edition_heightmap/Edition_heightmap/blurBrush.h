#pragma once
#ifndef blurBrush_h
#define blurBrush_h

#include <Windows.h>
#include <opencv2/core/types.hpp>

//class blurbrush : pinceau qui permet de flouter l'image par endroit
class BlurBrush
{
public:
	BlurBrush();
	BlurBrush(int rayon);
	void setRadius(int rayon);
	void blurPaint(cv::Mat& image, int x, int y);


private:
	int m_rayon;
};









#endif
