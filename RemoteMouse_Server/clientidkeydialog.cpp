#include "clientidkeydialog.h"
#include "ui_clientidkeydialog.h"

#include <QDebug>


ClientIdKeyDialog::ClientIdKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientIdKeyDialog)
{
    ui->setupUi(this);
    m_ids = ClientIdInterface::instance();

    ui->idKeyTable->setColumnCount(2);
    QStringList headerLabels;
    headerLabels.append("Client ID");
    headerLabels.append("Key");
    ui->idKeyTable->setHorizontalHeaderLabels(headerLabels);
}

ClientIdKeyDialog::~ClientIdKeyDialog()
{
    delete ui;
    clearChanges();
}

void ClientIdKeyDialog::generateKey()
{
    QList<QTableWidgetItem*> selection = ui->idKeyTable->selectedItems();
    if (!selection.isEmpty()) {
        QTableWidgetItem* selected = selection.at(0);
        QString newKey = m_ids->generateNewKey();
        NewKeyChange* change = new NewKeyChange(selected->text(), newKey);
        m_changes.append(change);
        ui->idKeyTable->item(selected->row(), 1)->setText(newKey);
    } else {
        QMessageBox::information(this, "No ID Selected",
                                 "A client must be selected to generate a key");
    }

}

void ClientIdKeyDialog::deleteId()
{
    QList<QTableWidgetItem*> selection = ui->idKeyTable->selectedItems();
    if (!selection.isEmpty()) {
        QTableWidgetItem* selected = selection.at(0);
        QString clientId = selected->text();
        DeleteIdChange* change = new DeleteIdChange(clientId);
        m_changes.append(change);
        ui->idKeyTable->removeRow(selected->row());
    } else {
        QMessageBox::information(this, "No ID Selected",
                                 "A client must be selected to generate a key");
    }
}

void ClientIdKeyDialog::addNewId()
{
    QString newId;
    bool result;
    bool errorOccurred = false;
    do {
        errorOccurred = false;
        newId = QInputDialog::getText(this, "Enter Name for New Client",
                                      "Client Name:", QLineEdit::Normal, QString(),
                                      &result, 0, Qt::ImhEmailCharactersOnly);
        if (m_ids->idExists(newId)) {
            QString errorMsg = "The name: ";
            errorMsg += newId;
            errorMsg += " already exists in the database.";
            QMessageBox::information(this, "Name Already Exists", errorMsg);
            errorOccurred = true;
        }
    } while (errorOccurred || !result);

    if (result) {
        int lastRow = ui->idKeyTable->rowCount();
        ui->idKeyTable->insertRow(lastRow);
        QTableWidgetItem* newItem = new QTableWidgetItem(newId);
        ui->idKeyTable->setItem(lastRow, 0, newItem);
        QString newKey = m_ids->generateNewKey();
        newItem = new QTableWidgetItem(newKey);
        ui->idKeyTable->setItem(lastRow, 1, newItem);
        AddIdChange* change = new AddIdChange(newId, newKey);
        m_changes.append(change);
    }
}

void ClientIdKeyDialog::renameId()
{
    QList<QTableWidgetItem*> selection = ui->idKeyTable->selectedItems();
    if (!selection.isEmpty()) {
        QTableWidgetItem* selected = selection.at(0);
        QString oldId = selected->text();
        QString newId;
        bool result;
        bool errorOccurred = false;
        do {
            errorOccurred = false;
            newId = QInputDialog::getText(this, "Enter New Name",
                                          "Client Name:", QLineEdit::Normal, QString(),
                                          &result, 0, Qt::ImhEmailCharactersOnly);

            if (m_ids->idExists(newId)) {
                QString errorMsg = "The name: ";
                errorMsg += newId;
                errorMsg += " already exists in the database.";
                QMessageBox::information(this, "Name Already Exists", errorMsg);
                errorOccurred = true;
            }
        } while (errorOccurred || !result);

        if (result) {
            selected->setText(newId);
            QString key = ui->idKeyTable->item(selected->row(), 1)->text();
            RenameIdChange* change = new RenameIdChange(oldId, newId, key);
            m_changes.append(change);
        }
    } else {
        QMessageBox::information(this, "No ID Selected", "Select a Client ID to rename");
    }
}

int ClientIdKeyDialog::exec()
{
    ui->idKeyTable->clearContents();
    initializeIdKeyTableData();
    return QDialog::exec();
}

void ClientIdKeyDialog::accept()
{
    applyChanges();
    QDialog::accept();
    qDebug() << "Saved.";
}

void ClientIdKeyDialog::reject()
{
    if (!m_changes.isEmpty()) {
        int ret = QMessageBox::question(this, "",
                                  "Are you sure you want to disregard changes?",
                                  QMessageBox::Ok | QMessageBox::Cancel,
                                  QMessageBox::Ok);
        if (ret == QMessageBox::Ok) {
            QDialog::reject();
            clearChanges();
        }
    } else
        QDialog::reject();

}

void ClientIdKeyDialog::initializeIdKeyTableData()
{
    QList<QString> ids = m_ids->getIdList();
    QList<QString> keys = m_ids->getKeyList();
    ui->idKeyTable->setRowCount(ids.size());

    for (int i = 0; i < ids.size(); i++) {
        QTableWidgetItem* item = new QTableWidgetItem(ids.at(i));
        ui->idKeyTable->setItem(i, 0, item);
        item = new QTableWidgetItem(keys.at(i));
        ui->idKeyTable->setItem(i, 1, item);
    }


}

void ClientIdKeyDialog::clearChanges()
{
    while (!m_changes.isEmpty())
        delete m_changes.takeFirst();
}

void ClientIdKeyDialog::applyChanges()
{
    while (!m_changes.isEmpty()) {
        AbstractIdKeyChange* change = m_changes.takeFirst();
        change->applyChange(m_ids);
        delete change;
    }
}
