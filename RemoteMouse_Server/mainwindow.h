#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "remotemouseserver.h"
#include "clientidkeydialog.h"

#define LISTEN_PORT 48048

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void startServer();
    void stopServer();
    void editIds();

private:
    QTableView* m_clients;

    RemoteMouseServer m_server;
    Ui::MainWindow *ui;
    ClientIdKeyDialog m_idKeyDialog;
};

#endif // MAINWINDOW_H
