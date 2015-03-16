#ifndef ADDPROVIDERDIALOG_H
#define ADDPROVIDERDIALOG_H

#include <QDialog>

namespace Ui {
class AddProviderDialog;
}

class AddProviderDialog : public QDialog
{
    Q_OBJECT

signals:
    void addProvider(QString);
private slots:
    void accept();
public:
    explicit AddProviderDialog(QWidget *parent = 0);
    ~AddProviderDialog();

private:
    Ui::AddProviderDialog *ui;
};

#endif // ADDPROVIDERDIALOG_H
