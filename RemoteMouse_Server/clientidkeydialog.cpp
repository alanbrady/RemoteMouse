#include "clientidkeydialog.h"
#include "ui_clientidkeydialog.h"
#include <QDebug>

ClientIdKeyDialog::ClientIdKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientIdKeyDialog)
{
    ui->setupUi(this);
    m_ids = ClientIdInterface::instance();
    
}

ClientIdKeyDialog::~ClientIdKeyDialog()
{
    delete ui;
}

void ClientIdKeyDialog::generateKey()
{
    
}

void ClientIdKeyDialog::addNewId()
{
    
}

void ClientIdKeyDialog::accept()
{
    hide();
    qDebug() << "Saved.";
}

void ClientIdKeyDialog::reject()
{ 
    hide();
    qDebug() << "Cancelled.";
}
