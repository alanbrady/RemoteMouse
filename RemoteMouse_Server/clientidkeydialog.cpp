#include "clientidkeydialog.h"
#include "ui_clientidkeydialog.h"

#include <QDebug>


ClientIdKeyDialog::ClientIdKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientIdKeyDialog)
{
    ui->setupUi(this);
    m_ids = ClientIdInterface::instance();

    m_changeMade = false;
}

ClientIdKeyDialog::~ClientIdKeyDialog()
{
    delete ui;
}

void ClientIdKeyDialog::generateKey()
{
    m_changeMade = true;
}

void ClientIdKeyDialog::deleteId()
{
    m_changeMade = true;
}

void ClientIdKeyDialog::addNewId()
{
    m_changeMade = true;
}

int ClientIdKeyDialog::exec()
{
//    qDebug() << "Dialog started.";
    m_changeMade = false;
    return QDialog::exec();
}

void ClientIdKeyDialog::accept()
{
//    hide();
    QDialog::accept();
    qDebug() << "Saved.";
}

void ClientIdKeyDialog::reject()
{
    if (m_changeMade) {
        int ret = QMessageBox::question(this, "",
                                  "Are you sure you want to disregard changes?",
                                  QMessageBox::Ok | QMessageBox::Cancel,
                                  QMessageBox::Ok);
        if (ret == QMessageBox::Ok)
                QDialog::reject();
    } else
        QDialog::reject();

}
