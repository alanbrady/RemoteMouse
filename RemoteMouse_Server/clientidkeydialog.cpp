#include "clientidkeydialog.h"
#include "ui_clientidkeydialog.h"

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
    
}

void ClientIdKeyDialog::reject()
{
    
}
