#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>
#include <producttablemodel.h>
#include <addproductdialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void productIndexChanged();
private slots:
    void handleSelectionChanged(const QItemSelection&);
    void focusToProduct();
    void refreshTransactionList();
    void on_inputPushButton_clicked();

    void on_addProductButton_clicked();
    void confirmAddProduct(QString,QString,QString);
private:
    Ui::MainWindow *ui;
    ProductTableModel *productModel;
    QSqlTableModel *transactionModel;
    int productIndex;
    void showError(const QSqlError &err);
};

#endif // MAINWINDOW_H
