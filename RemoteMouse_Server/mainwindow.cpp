#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_clients = ui->table_clients;

    connect(ui->button_start, SIGNAL(clicked()), this, SLOT(startServer()));
    connect(ui->button_stop, SIGNAL(clicked()), this, SLOT(stopServer()));

    ui->statusBar->showMessage("Remote Mouse initialized.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startServer()
{
    if (!m_server.listen(QHostAddress::Any, LISTEN_PORT)) {
        ui->statusBar->showMessage("Error: unable to start server!");
    } else {
        QString msg("Server is now listening on port: ");
        msg += QString::number(LISTEN_PORT);
        ui->statusBar->showMessage(msg);
    }
}

void MainWindow::stopServer()
{
    m_server.close();
    ui->statusBar->showMessage("Server is not listening for connections.");

}
