#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    RemoteMouseServer server;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
