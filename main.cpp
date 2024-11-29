#include "window.h"
#include "BasedeD.h" // Inclure votre fichier d'en-tête

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    GestionnaireBase gestionnaire;
    gestionnaire.connecterBaseDeDonnees(); // appelle de la fonction  de connexion

    w.show();
    return a.exec();
}
