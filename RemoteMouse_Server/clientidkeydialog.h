#ifndef CLIENTIDKEYDIALOG_H
#define CLIENTIDKEYDIALOG_H

#include <QDialog>

namespace Ui {
class ClientIdKeyDialog;
}

class ClientIdKeyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientIdKeyDialog(QWidget *parent = 0);
    ~ClientIdKeyDialog();

private:
    Ui::ClientIdKeyDialog *ui;
};

#endif // CLIENTIDKEYDIALOG_H
