// Edition_heightmap.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//


#include <string>
#include <Windows.h>
#include <GL/GL.h>
#include <GL/glu.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include <QtWidgets/qpushbutton.h>
#include <QtCore/qt_windows.h>

#include "circlebrush.h"
#include "blurBrush.h"

using namespace std;
using namespace cv;

//---------VARIABLES-----------------
Mat img;
int X = 0; //nombre de colonnes
int Y = 0; //nombre de lignes
int rayonPinceau = 20;
Scalar couleur(0, 0, 0, 0);
/*mode d'edition
0 dessin
1 flou*/
int mode = 0; 

//Outil pinceau
CircleBrush pinceau(10, 20, 0);
BlurBrush pinceauFlou(10);

//---------------------------CALLBACK-------------------------------

//callback souris pour le tracé 
void SourisCallback(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN && mode==0)
    {
       /* circle(img,
            Point(x, y),  //center
            rayonPinceau,                 //radius
            couleur,      //color
            FILLED,
            LINE_8);*/
        pinceau.paint(x, y, img);
    }
    if (event == EVENT_LBUTTONDOWN && mode == 1)
    {
        pinceauFlou.blurPaint(img, x, y);
    }
};

//callback pour le mode d'edition
void trackbarCallbackMode(int value, void* userdata)
{
    mode = value;
}

//callbacks bouttons pour la taille du pinceau
//Taille PLUS
void callbackButtonPlus(int state, void* userdata)
{
    if (state == -1)    //-1 pour un push_button
    {
        rayonPinceau += 10;
        pinceau.setRadiusExt(pinceau.getRadiusExt() + 10);
    }
}

//Taille MOINS
void callbackButtonMinus(int state, void* userdata)
{
    if (state == -1)    //-1 pour un push_button
    {
        rayonPinceau -= 10;
        pinceau.setRadiusExt(pinceau.getRadiusExt() - 10);
    }
}

//Couleur du pinceau en niveau de gris
void trackbarCallbackColor(int value, void* userdata)
{
    couleur = (value, value, value);
    pinceau.setColor(value);
}

//taille du pinceau 
void trackbarCallbackRadiusExt(int value, void* userdata)
{
    rayonPinceau = value;
    pinceau.setRadiusExt(value);

}

void trackbarCallbackRadiusInt(int value, void* userdata)
{
    rayonPinceau = value;
    pinceau.setRadiusInt(value);
    pinceauFlou.setRadius(value);
}

int main()
{
    cout << "saisir le chemin du fichier de la heightmap voulue" << endl;
    string image_path;
    cin >> image_path;
    img = imread(image_path, IMREAD_GRAYSCALE);
    X = img.rows;
    Y = img.cols;
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    cout << "taille :" << img.rows << "x" << img.cols << endl;

    namedWindow("Image");
    
    //callback souris pour le tracé
    setMouseCallback("Image", SourisCallback);

    //-----NECESSITE QT ------
    //callback bouton pour taille du pinceau 
    //createButton("+", callbackButtonPlus, NULL, QT_PUSH_BUTTON, 0);
    //createButton("-", callbackButtonMinus, NULL, QT_PUSH_BUTTON, 0);

    //Nouvelle fenêtre pour ne pas encombrer avec les trackbars
    namedWindow("Tools");
    moveWindow("Tools", 1000, 100);
    resizeWindow("Tools", 600, 200);

    //-----SANS QT -----
    //callabck pour le mode d'edition
    /*pour remplacer les boutons, on utilise une trackar avec seulement 2 positions : dessin ou flou*/
    createTrackbar("mode d'edition: dessin --- flou", "Tools", &mode, 1, trackbarCallbackMode);

    //callback pour la couleur du tracé
    createTrackbar("color", "Tools", NULL, 255, trackbarCallbackColor);
    //callback pour la taille du pinceau
    createTrackbar("radiusInt", "Tools", &rayonPinceau, X/8, trackbarCallbackRadiusInt); //&rayonpinceau obsolete, a changer
    createTrackbar("radiusExt", "Tools", &rayonPinceau, X/8, trackbarCallbackRadiusExt);

    //callback clavier mode d'edition
    

    while (waitKey(20) != 27)  // wait until ESC is pressed
    {
        imshow("Image", img);   //force le réaffichage
    }
    imwrite("image_modifiee.png", img);
    return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
