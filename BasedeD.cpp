// BasedeD.cpp

#include "BasedeD.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool GestionnaireBase::connecterBaseDeDonnees() {


    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");  // Driver MySQL
    db.setHostName("localhost");
    db.setDatabaseName("BANQUE");
    db.setUserName("TPQT");
    db.setPassword("");

    if (!db.open()) {
        qDebug() << "Base de donnée pas ouverte!";
        return false;
    }

    qDebug() << "Base de donnée ouverte!";
    return true;
}


bool GestionnaireBase::verifierIdentifiants(const QString &id, const QString &mdp) {
    // Implémentation de la vérification des identifiants
    QSqlQuery query;
    query.prepare("SELECT * FROM clients WHERE id = :id AND mdp = :mdp");
    query.bindValue(":id", id);
    query.bindValue(":mdp", mdp);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
        return false;
    }

    return query.next();
}


QSqlQuery GestionnaireBase::executerRequete(const QString &requete) {
    QSqlQuery query;
    if (!query.exec(requete)) {
        qDebug() << "Erreur d'exécution de la requête: " << query.lastError().text();
    }
    return query;
}

QSqlQuery GestionnaireBase::Requete(const QString &requete, const QVariantList &parametres) {
    QSqlQuery query;
    query.prepare(requete);
    for (int i = 0; i < parametres.size(); ++i) {
        query.addBindValue(parametres.at(i));
    }

    if (!query.exec()) {
        qDebug() << "Erreur d'exécution de la requête avec paramètres: " << query.lastError().text();
    }
    return query;
}
