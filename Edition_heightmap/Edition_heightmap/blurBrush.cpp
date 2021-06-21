#include "blurBrush.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

BlurBrush::BlurBrush() : m_rayon(10)
{}

BlurBrush::BlurBrush(int rayon) : m_rayon(rayon)
{}

void BlurBrush::setRadius(int rayon)
{
	m_rayon = rayon;
}

void BlurBrush::blurPaint(cv::Mat& image, int x, int y)
{
	//cv::Point point(x, y);
	//blur(image, image, cv::Size(1,1), point);
	//cv::Rect region(x, y, 50, 50);
	//cv::GaussianBlur(image(region), image(region), cv::Size(0, 0), 4);
	//on copie l'image pour que les modifications n'afectent pas les calculs suivants
	cv::Mat imageFinale(image.rows, image.cols, image.type());
	image.copyTo(imageFinale);
	//on définit le filtre : filtre gaussien 3x3
	uint8_t arrayData[3][3] = { 
		{1, 2 , 1}, 
		{2 , 4, 2 },
		{ 1 , 2 , 1 } };
	cv::Mat filtre = cv::Mat(3, 3, CV_8U, &arrayData);
	std::cout << filtre.at < uchar>(1, 1);
	for (int i = 0; i < image.cols; i++)
	{
		for (int j = 0; j < image.rows; j++)
		{
			if (((x - i) * (x - i) + (y - j) * (y - j)) < (m_rayon * m_rayon))
				//le pixel est dans la zone floutée : on applique le filtre
			{
				//on moyenne avec les cases alentour
				int somme = 0;
				int sommeD = 0;
				//on s'intéresse au carré de 9 pixels autour 
				for (int k = 0; k < 3 && (-1 < j + k - 1 ) && (j+k-1 < image.rows); k++)
				{
					for (int l = 0; l < 3 && (-1 < i + l - 1 ) && (i + l - 1 < image.cols); l++)
					{
						//on applique le filtre
						somme += (image.at<uchar>(j+k-1, i+l-1) * (filtre.at<uchar>(k,l)));
						std::cout << "somme " << somme << std::endl;
					}
				}
				imageFinale.at<uchar>(j, i) = somme/16;
			}
		}
	}
	imageFinale.copyTo(image);
	
}