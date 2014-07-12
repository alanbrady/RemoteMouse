#ifndef CLIENTIDKEYDIALOG_H
#define CLIENTIDKEYDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "clientidinterface.h"

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
    void accept();
    void reject();
    
private:
    Ui::ClientIdKeyDialog *ui;
    ClientIdInterface* m_ids;

    bool m_changeMade;
};

#endif // CLIENTIDKEYDIALOG_H
