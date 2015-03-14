#ifndef ADDPRODUCTDIALOG_H
#define ADDPRODUCTDIALOG_H

#include <QDialog>

namespace Ui {
class AddProductDialog;
}

class AddProductDialog : public QDialog
{
    Q_OBJECT

signals:
    void addProduct(QString, QString, QString);
private slots:
    void accept();
public:
    explicit AddProductDialog(QWidget *parent = 0);
    ~AddProductDialog();

private:
    Ui::AddProductDialog *ui;
};

#endif // ADDPRODUCTDIALOG_H
