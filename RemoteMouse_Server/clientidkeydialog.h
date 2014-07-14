#ifndef CLIENTIDKEYDIALOG_H
#define CLIENTIDKEYDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QQueue>
#include <QInputDialog>
#include "clientidinterface.h"
#include "abstractidkeychange.h"
#include "addidchange.h"
#include "renameidchange.h"
#include "deleteidchange.h"
#include "newkeychange.h"

namespace Ui {
class ClientIdKeyDialog;
}

class ClientIdKeyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientIdKeyDialog(QWidget *parent = 0);
    ~ClientIdKeyDialog();

    int exec();

public slots:
    void generateKey();
    void deleteId();
    void addNewId();
    void renameId();
    void accept();
    void reject();
    
private:
    Ui::ClientIdKeyDialog *ui;
    ClientIdInterface* m_ids;
    QQueue<AbstractIdKeyChange*> m_changes;

    void initializeIdKeyTableData();
    void clearChanges();
    void applyChanges();
};

#endif // CLIENTIDKEYDIALOG_H
