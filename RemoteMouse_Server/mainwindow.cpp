#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startServer()
{
    if (!server.listen(QHostAddress::Any, LISTEN_PORT)) {
        // TODO error
    }
}

void MainWindow::stopServer()
{
    server.close();
}
