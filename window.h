#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);

private slots:
    void echoChanged(bool checked);
    void valide();
    void connexion();
    void MENU();
    void versement();
    void debiter();
    void virement();
    void nouvInfo();


private:
    QLineEdit *idLineEdit;
    QLineEdit *passwordLineEdit;
    QCheckBox *montreMDPCheckBox;
    QPushButton *connexionButton;
    QLabel *statusLabel;
    QLabel *messageBienvenue = new QLabel;
    QLabel *messageSolde = new QLabel;

    int client_id;
};

#endif // WINDOW_H
