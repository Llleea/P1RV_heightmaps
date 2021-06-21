// P1RV_heightmap_1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.

//  Created by Hugo ALLEMAND and Léa PREMONT on 19/10/2020.

//  Copyright © 2020 Hugo ALLEMAND & Léa PREMONT. All rights reserved.

//


#include <string>
#include <Windows.h>
#include <GL/GL.h>
#include <GL/glu.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include<GLFW/glfw3.h>


using namespace std;
using namespace cv;

Mat img;

//variables pour l'affichage 
int windowW = 640;
int windowH = 640;
float focale = 65.0f;
float nearp = 0.0f;
float farp = 100.0f;

//variables positionnement de la caméra
float camPosX = 0;
float camPosY = 0;
float camPosZ = 255;
float camAtX = 500;
float camAtY = 500;
float camAtZ = 0; 

//position de la souris et sensibilité pour le deplacement
double mouseX = -1;
double mouseY = -1;
float sensibilite = 0.004;

//angles de rotation (coordonees spheriques) avec leurs increments 
float theta = 0;
float phi = 0;
float deltaTheta = 0.0;
float deltaPhi = 0.0;

//increments de deplacement
float deltaMove = 0;
float deltaStrafe = 0;
float deltaUp = 0;

//vecteur avant 
float forwardViewX = 1;
float forwardViewY = 1;
float forwardViewZ = 1;

//vecteur up world
float upX = 0;
float upY = 0;
float upZ = 1;

//vecteur "a droite"
float rightX = 1;
float rightY = 1;
float rightZ = 1;

int precision = 1; //precision d'affichage = pas; on considere un triangle sur pas 

//echelle arbitraire pour un premier essai
double echelleCouleur[5][3] = {
    {1,1,0},
    {0,1,0},
    {0,1,1},
    {0,0,1},
    {1,1,1}
};

//echelle avec degrade de couleur realiste
double echelleCouleurHauteur[9][3] = {
    {0, 0.65, 1},
    {0.11, 0.67, 0.11},
    {0.27, 0.83, 0.23},
    {0.86, 0.84, 0},
    {0.78, 0.57, 0.13},
    {0.65, 0.45, 0.12},
    {0.59, 0.62, 0.62},
    {0.78, 0.9, 0.87},
    {1, 1, 1}
};

//Echelle de couleur qui donne des 'rayures', des lignes de niveaux 
void choixCouleur(double& r, double& g, double& b, int hauteur)

{
    hauteur = hauteur / 9;
    r = echelleCouleur[hauteur][0];
    g = echelleCouleur[hauteur][1];
    b = echelleCouleur[hauteur][2];
}

//Echelle de couleur par hauteur
void choixCouleurHauteur(double& r, double& g, double& b, int hauteur)
{
    hauteur = hauteur / (255/9);
    r = echelleCouleurHauteur[hauteur][0];
    g = echelleCouleurHauteur[hauteur][1];
    b = echelleCouleurHauteur[hauteur][2];
}

//callack clavier : deplacement de la camera dans le monde
void clavierCallback(GLFWwindow* window, int touche, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (touche)
        {
        case GLFW_KEY_LEFT:
            deltaStrafe += 5;           
            break;
        case GLFW_KEY_RIGHT:
            deltaStrafe -= 5;
            break;
        case GLFW_KEY_DOWN:
            deltaMove += 5;
            break;
        case GLFW_KEY_UP:
            deltaMove -= 5;
            break;
        case GLFW_KEY_I:
            deltaUp -= 5;
            break;
        case GLFW_KEY_O:
            deltaUp += 5;
            break;
        }
    }
    
}

//execution du deplacement de la camera
void moveCamera(float dMove, float dStrafe, float dUp)
{
    camPosX += dMove * sin(theta) + dStrafe * cos(theta); //deplacement droite gauche
    camPosZ += dUp;//deplacement haut bas
    camPosY += dStrafe * sin(-theta) + dMove * cos(theta); //deplacement avant arriere
    //Mise a jour de la position
    camAtX = camPosX + forwardViewX;
    camAtY = camPosY + forwardViewY;
    camAtZ = camPosZ + forwardViewZ;
}

//callback souris : pour changer l'orientation de la camera
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {//on presse le bouton gauche, on mémorise la position
            glfwGetCursorPos(window, &mouseX, &mouseY); 
        }
        else
        {//on relache le bouton gauche, on actualise les angles, on dit qu'on a pas cliqué
            mouseX = -1;
            mouseY = -1;
            theta += deltaTheta;
            phi += deltaPhi;
        }
    }
}

//fonction pour normaliser un vecteur de composantes x, y z
void normaliser(float& x, float& y, float& z)
{
    float facteur = sqrt(x * x + y * y + z * z);
    x /= facteur;
    y /= facteur;
    z /= facteur;
}

//fonction qui effectue le produit vectoriel de (a,b,c) par (x,y,z) et le met dans (s,t,u)
void produitVectoriel(float& s, float& t, float& u, float& x, float& y, float& z, float& a, float& b, float& c)
{
    s= b * z - c * y;
    t = c * x - a * z;
    u = a * y - b * x;
}

//callback souris pour l'orientation de la camera
void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    // On ne fait quelque chose que si l'utilisateur
    // a deja clique quelque part avec le bouton gauche
    if (mouseX >= 0 || mouseY >= 0) 
    {
        // mise a jour des deltas des angles theta et phi
        deltaTheta = -(x - mouseX) * sensibilite;
        deltaPhi = -(y - mouseY) * sensibilite;

        // Calcul du nouveau vecteur vision :
        forwardViewY = -cos(theta + deltaTheta);
        forwardViewX = -sin(theta + deltaTheta);
        forwardViewZ = -cos(phi + deltaPhi);

        normaliser(forwardViewX, forwardViewY, forwardViewZ);

        produitVectoriel(rightX, rightY, rightZ, forwardViewX, forwardViewY, forwardViewZ, upX, upY, upZ);
        
        camAtX = camPosX + forwardViewX;
        camAtZ = camPosZ + forwardViewZ;
        camAtY = camPosY + forwardViewY;
    }
}

 //fonction d'affichage du terrain
void afficherTerrain3d()
{
//Deplacement de la camera 
   moveCamera(deltaMove, deltaStrafe, deltaUp);

//remise a zero
    deltaMove = 0;
    deltaStrafe = 0;
    deltaUp = 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //placement de la camera
    gluLookAt(camPosX, camPosY, camPosZ, camAtX, camAtY, camAtZ, 0, 0, 1);

    //attention ici au rang max, -1 car on fait des i+1 et j+1
    for (int i = 0; i < img.rows - precision - 1; i = i + precision)
    {
        for (int j = 0; j < img.cols - precision - 1; j = j + precision)
        {
            //declaration de variable necessaires
            double couleurR, couleurG, couleurB;
            int hauteur;

            //on commence le triangle du haut sur une ligne
            glBegin(GL_TRIANGLES);

            //Premier vertex du triangle
            hauteur = (int)img.at<uchar>(i, j);
            choixCouleurHauteur(couleurR, couleurG, couleurB, hauteur);
            glColor3f(couleurR, couleurG, couleurB);
            glVertex3f(i, j, hauteur);
            //Deuxieme vertex du triangle
            hauteur = (int)img.at<uchar>(i, j + precision);
            choixCouleurHauteur(couleurR, couleurG, couleurB, hauteur);
            glColor3f(couleurR, couleurG, couleurB);
            glVertex3f(i, j + precision, hauteur);
            //Troisieme vertex du triangle
            hauteur = (int)img.at<uchar>(i + precision, j);
            choixCouleurHauteur(couleurR, couleurG, couleurB, hauteur);
            glColor3f(couleurR, couleurG, couleurB);
            glVertex3f(i + precision, j, hauteur);

            glEnd();

            //on dessine maintenant le triangle du bas sur une ligne
            glBegin(GL_TRIANGLES);

            //Premier vertex du triangle
            hauteur = (int)img.at<uchar>(i + precision, j);
            choixCouleurHauteur(couleurR, couleurG, couleurB, hauteur);
            glColor3f(couleurR, couleurG, couleurB);
            glVertex3f(i + precision, j, hauteur);
            //Deuxieme vertex du triangle
            hauteur = (int)img.at<uchar>(i, j + precision);
            choixCouleurHauteur(couleurR, couleurG, couleurB, hauteur);
            glColor3f(couleurR, couleurG, couleurB);
            glVertex3f(i, j + precision, hauteur);
            //Troisieme vertex du triangle
            hauteur = (int)img.at<uchar>(i + precision, j + precision);
            choixCouleurHauteur(couleurR, couleurG, couleurB, hauteur);
            glColor3f(couleurR, couleurG, couleurB);
            glVertex3f(i + precision, j + precision, hauteur);
            glEnd();
        }
    }
    glFlush();
}


//redimensionnement de la fenetre
void redimensionner(int w, int h)
{
    // Garde les valeurs
    windowW = w;
    windowH = h;
    // eviter une division par 0
    if (windowH == 0)
        windowH = 1;
    float ratio = (float)windowW / (float)windowH;
    std::cout << "Ratio : " << ratio << std::endl;
    // Projection
    glMatrixMode(GL_PROJECTION);

    // Resetting matrix
    glLoadIdentity();

    // Viewport
    glViewport(0, 0, windowW, windowH);

    // Mise en place de la perspective
    // TODO : peut-on changer le ratio ici pour un meilleur resultat ? --> ratio plutot que 4/3
    gluPerspective(focale, ratio, nearp, farp);

    // Retourne a la pile modelview
    glMatrixMode(GL_MODELVIEW);
}



int main()

{
    cout << "saisir la precision (1 par defaut mais peut nécessiter un long temps de calcul, 15 fait l'affaire)" << endl;
    cin >> precision;

    //Partie openCV
    //lecture de l'image
    cout << "saisir le chemin du fichier de la heightmap voulue" << endl;
    string image_path;
    cin >> image_path;
    img = imread(image_path, IMREAD_GRAYSCALE);
    camPosX = img.rows;
    camPosY = img.cols;
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    imshow("Display window", img);
    cout << "taille :" << img.rows << "x" << img.cols << endl;


    //Partie GLFW & OpenGL
    //initialiser la librairie
    GLFWwindow* window;

    if (!glfwInit())
    {
       return -1;
    }

    //creeation de la fenetre
    window = glfwCreateWindow(640, 640, "Terrain 3D", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //appel des callbacks
    glfwSetKeyCallback(window, clavierCallback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //actualisation de l'affichage
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        redimensionner(width, height);

        //Drawing
        afficherTerrain3d();

        //swap buffers
        glfwSwapBuffers(window);

        //pool for and process event
        glfwPollEvents();
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    glfwTerminate();
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
