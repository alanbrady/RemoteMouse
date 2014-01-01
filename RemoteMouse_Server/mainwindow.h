#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "remotemouseserver.h"

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

private:
    QTableView* m_clients;

    RemoteMouseServer m_server;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
