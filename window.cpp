#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "window.h"
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include <QDialog>
#include "BasedeD.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)  // Utilisez QMainWindow comme classe de base
{


    setWindowTitle("MAZE BANK");
    resize(400, 300);


    idLineEdit = new QLineEdit;
    idLineEdit->setPlaceholderText("Identifiant");
    idLineEdit->setMaxLength(9);

    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setPlaceholderText("Mot de passe");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    montreMDPCheckBox = new QCheckBox("Afficher Mot de Passe");
    connect(montreMDPCheckBox, &QCheckBox::toggled, this, &Window::echoChanged);

    connexionButton = new QPushButton("Connexion");
    connexionButton->setEnabled(false);
    connect(connexionButton, &QPushButton::clicked, this, &Window::connexion);

    statusLabel = new QLabel;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(idLineEdit);
    mainLayout->addWidget(passwordLineEdit);
    mainLayout->addWidget(montreMDPCheckBox);
    mainLayout->addWidget(connexionButton);
    mainLayout->addWidget(statusLabel);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);  // Définir le widget central pour QMainWindow

    connect(idLineEdit, &QLineEdit::textChanged, this, &Window::valide);
    connect(passwordLineEdit, &QLineEdit::textChanged, this, &Window::valide);


}

void Window::echoChanged(bool checked)
{
    passwordLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void Window::valide()
{
    connexionButton->setEnabled(!idLineEdit->text().isEmpty() && !passwordLineEdit->text().isEmpty());
}

void Window::connexion() {
    QString id = idLineEdit->text();
    QString mdp = passwordLineEdit->text();

    if (GestionnaireBase::verifierIdentifiants(id, mdp)) {
        statusLabel->setText("Connexion réussie !");
        statusLabel->setStyleSheet("color: green;");
        client_id = id.toInt();  // Stocker l'ID du client

        connect(connexionButton, &QPushButton::clicked, this, [=]() {
            this->close();
            MENU();
        });
    } else {
        statusLabel->setText("Identifiant et/ou mot de passe incorrect !");
        statusLabel->setStyleSheet("color: red;");
    }
}

void Window::MENU()
{
    QMainWindow *menuWindow = new QMainWindow(this); // Renommez pour éviter la confusion avec la fonction nouvInfo
    menuWindow->setWindowTitle("Accueil");
    menuWindow->resize(400, 300);

    QGridLayout *menulayout = new QGridLayout;
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(menulayout);
    menuWindow->setCentralWidget(centralWidget);

    messageBienvenue = new QLabel;
    menulayout->addWidget(messageBienvenue, 1, 1);

    messageSolde = new QLabel;
    menulayout->addWidget(messageSolde, 2, 1);
    messageBienvenue->setAlignment(Qt::AlignCenter);
    messageSolde->setAlignment(Qt::AlignCenter);

    QMenuBar *menuBar = new QMenuBar(menuWindow);

    QMenu *actionCompte = new QMenu("Opérations", menuWindow);
    QAction *versementAction = new QAction("Versement", menuWindow);
    QAction *debitAction = new QAction("Debiter", menuWindow);
    QAction *virementAction = new QAction("Virement", menuWindow);

    actionCompte->addAction(versementAction);
    actionCompte->addAction(debitAction);
    actionCompte->addAction(virementAction);

    connect(versementAction, &QAction::triggered, this, [=]() {
        menuWindow->hide();  // Ferme la fenêtre menuWindow
        this->versement();   // Appelle la fonction versement
    });

    connect(debitAction, &QAction::triggered, this, [=]() {
        menuWindow->hide();  // Ferme la fenêtre menuWindow
        this->debiter();     // Appelle la fonction debiter
    });

    connect(virementAction, &QAction::triggered, this, [=]() {
        menuWindow->hide();  // Ferme la fenêtre menuWindow
        this->virement();    // Appelle la fonction virement
    });

    menuBar->addMenu(actionCompte);
    menuWindow->setMenuBar(menuBar);

    QPushButton *deco = new QPushButton("Déconnexion");
    connect(deco, &QPushButton::clicked, this, [=]() {
        menuWindow->close(); // Fermer la fenêtre menuWindow
        this->show(); // Réafficher la fenêtre principale si nécessaire
    });

    menulayout->addWidget(deco, 4, 1);

    menuWindow->show();

    nouvInfo(); // Appel à nouvInfo() après la configuration de l'interface utilisateur
}


void Window::versement() {
    bool ok;
    double montant = QInputDialog::getDouble(this, tr("Versement"), tr("Montant à verser:"), 0, 0, 10000, 2, &ok);
    if (ok) {
        QSqlQuery query;
        query.prepare("UPDATE comptes SET solde = solde + :montant WHERE rib_client = (SELECT rib FROM clients WHERE id = :id)");
        query.bindValue(":montant", montant);
        query.bindValue(":id", client_id);

        if (query.exec()) {
            QMessageBox::information(this, "Crédit", "Montant crédité avec succès.");
            nouvInfo();
            MENU();
        } else {
            QMessageBox::warning(this, "Erreur", "Erreur lors du crédit: " + query.lastError().text());
            MENU();
        }
    }
}

void Window::debiter() {
    bool ok;
    double montant = QInputDialog::getDouble(this, tr("Versement"), tr("Montant à debiter:"), 0, 0, 10000, 2, &ok);
    if (ok) {
        QSqlQuery query;
        query.prepare("UPDATE comptes SET solde = solde - :montant WHERE rib_client = (SELECT rib FROM clients WHERE id = :id)");
        query.bindValue(":montant", montant);
        query.bindValue(":id", client_id);

        if (query.exec()) {
            QMessageBox::information(this, "Crédit", "Montant crédité avec succès.");
            nouvInfo();
            MENU();
        } else {
            QMessageBox::warning(this, "Erreur", "Erreur lors du crédit: " + query.lastError().text());
            MENU();
        }
    }
}

void Window::virement() {
    bool ok;
    double RIBdestinataire = QInputDialog::getDouble(this, tr("Virement"), tr("RIB du destinataire:"), 0, 0, 9000000, 0, &ok);
    if (ok && RIBdestinataire) {  // Vérifie que le RIB est valide

        QSqlQuery Query;
        Query.prepare("SELECT client.nom, client.prenom, compte.solde FROM clients client JOIN comptes compte ON client.id = compte.id WHERE client.rib = :rib");
        Query.bindValue(":rib", RIBdestinataire);

        if (!Query.exec() || !Query.next()) {
            QMessageBox::warning(this, "Erreur", "RIB du bénéficiaire introuvable.");
            MENU();
            return;
        }

        QString nom = Query.value(0).toString();
        QString prenom = Query.value(1).toString();

        double montant = QInputDialog::getDouble(this, tr("Virement"), tr("Montant à transférer:"), 0, 0, 100000, 2, &ok); // création d'un nouveau input
        if (ok) {
            QSqlQuery verifQuery;
            verifQuery.prepare("SELECT solde FROM comptes WHERE rib_client = (SELECT rib FROM clients WHERE id = :id)");
            verifQuery.bindValue(":id", client_id);

            if (!verifQuery.exec() || !verifQuery.next()) {
                QMessageBox::warning(this, "Erreur", "Erreur lors de la vérification du solde.");
                MENU();
                return;
            }

            double solde = verifQuery.value(0).toDouble(); //recuparation DU SOLDE via LA PREMIER REQUETE

            if (solde < montant) {  // Correction pour la vérification du solde
                QMessageBox::warning(this, "Erreur", "Solde insuffisant.");
                MENU();
                return;
            }


            //Validation et application du virement
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Confirmation", QString("Vous voulez bien transférer %1 € à %2 %3 avec le RIB %4 ?").arg(montant).arg(prenom).arg(nom).arg(RIBdestinataire), QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                QSqlDatabase::database().transaction();
                QSqlQuery debitQuery;
                //RETRAIT MOTANT RIB ENVOYEUR
                debitQuery.prepare("UPDATE comptes SET solde = solde - :montant WHERE rib_client = (SELECT rib FROM clients WHERE id = :id) AND solde >= :montant");
                debitQuery.bindValue(":montant", montant);
                debitQuery.bindValue(":id", client_id);

                //requet executer et à bien changer quelque chose la BDD
                if (debitQuery.exec() && debitQuery.numRowsAffected()) {
                    QSqlQuery creditQuery;
                //AJOUT MOTANT RIB DESTINATAIRE
                    creditQuery.prepare("UPDATE comptes SET solde = solde + :montant WHERE rib_client = :rib");
                    creditQuery.bindValue(":montant", montant);
                    creditQuery.bindValue(":rib", RIBdestinataire);


                //requet executer et à bien changer quelque chose la BDD
                    if (creditQuery.exec() && creditQuery.numRowsAffected()) {
                        QSqlDatabase::database().commit();
                        QMessageBox::information(this, "Virement", "Virement effectué avec succès.");
                        nouvInfo();
                        MENU();
                    } else {
                        QMessageBox::warning(this, "Erreur", "Erreur lors du crédit: " + creditQuery.lastError().text());
                        MENU();
                    }
                } else {
                    QMessageBox::warning(this, "Erreur", "Erreur lors du débit: " + debitQuery.lastError().text());
                    nouvInfo();
                    MENU();
                }
            }
        }
    }
}

void Window::nouvInfo()
{
    QSqlQuery query;
    query.prepare("SELECT client.nom, client.prenom, compte.solde FROM clients client JOIN comptes compte ON client.rib = compte.rib_client WHERE client.id = :id");
    query.bindValue(":id", client_id);

    if (query.exec() && query.next()) {
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        double solde = query.value(2).toDouble();

        messageBienvenue->setText(QString("Bienvenue %1 %2.").arg(prenom).arg(nom));
        messageSolde->setText(QString("Solde : %1 €").arg(solde));
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
        messageBienvenue->setText("Erreur lors de l'exécution de la requête.");
    }
}





