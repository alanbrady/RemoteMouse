#ifndef CLIENTIDKEYDIALOG_H
#define CLIENTIDKEYDIALOG_H

#include <QDialog>
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

public slots:
    void generateKey();
    void addNewId();
    void accept();
    void reject();
    
private:
    Ui::ClientIdKeyDialog *ui;
    ClientIdInterface* m_ids;
};

#endif // CLIENTIDKEYDIALOG_H
