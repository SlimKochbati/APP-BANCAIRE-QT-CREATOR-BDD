#ifndef BASEDED_H
#define BASEDED_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QString>
#include <QDebug>

class GestionnaireBase {
public:
    bool connecterBaseDeDonnees();
    static bool verifierIdentifiants(const QString &rib, const QString &mdp);
    QSqlQuery executerRequete(const QString &requete);
    QSqlQuery Requete(const QString &requete, const QVariantList &parametres);

};

#endif // BASEDED_H
