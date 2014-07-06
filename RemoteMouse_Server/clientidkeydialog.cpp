#include "clientidkeydialog.h"
#include "ui_clientidkeydialog.h"

ClientIdKeyDialog::ClientIdKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientIdKeyDialog)
{
    ui->setupUi(this);
}

ClientIdKeyDialog::~ClientIdKeyDialog()
{
    delete ui;
}
