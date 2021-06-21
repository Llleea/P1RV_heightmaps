#include "circlebrush.h"
#include <opencv2/core.hpp>

using namespace cv; 

/*Definition des methodes de la classe circlebrush*/

CircleBrush::CircleBrush():m_radiusInt(10), m_radiusExt(20), m_color(0)
{}

CircleBrush::CircleBrush(int radiusInt, int radiusExt, int color): m_radiusInt(radiusInt), m_radiusExt(radiusExt), m_color(color)
{}

void CircleBrush::setRadiusInt(int newValue)
{
	m_radiusInt = newValue;
}

void CircleBrush::setRadiusExt(int newValue)
{
	m_radiusExt = newValue;
}

void CircleBrush::setColor(int newColor)
{
	m_color = newColor;
}

int CircleBrush::getRadiusInt() const
{
	return m_radiusInt;
}

int CircleBrush::getRadiusExt() const
{
	return m_radiusExt;
}


void CircleBrush::paint(int x, int y, cv::Mat &image) const
{
	for (int i = 0; i < image.cols; i++)
	{
		for (int j = 0; j < image.rows; j++)
		{
			if (((x - i) * (x - i) + (y - j) * (y - j)) < (m_radiusInt * m_radiusInt))
				//le pixel est dans le rayon interieur : on le peint opaque
			{
				image.at<uchar>(j, i) = m_color;
			}
			if (((x - i) * (x - i) + (y - j) * (y - j)) <= (m_radiusExt * m_radiusExt) && ((x - i) * (x - i) + (y - j) * (y - j)) >= (m_radiusInt * m_radiusInt))
				//le pixel est sur la zone de dégrade, les bords du pinceau, on melange les couleurs suivant la distance au bord
			{
				image.at<uchar>(j, i) = m_color + (image.at<uchar>(j, i) - m_color) * (sqrt((x - i) * (x - i) + (y - j) * (y - j)) - m_radiusInt) / (m_radiusExt - m_radiusInt);
			}
		}
	}
}
