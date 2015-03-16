#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>
#include <producttablemodel.h>
#include <addproductdialog.h>
#include <addproviderdialog.h>
#include <providertablemodel.h>

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
    void handleProviderChanged(int);
    void refreshTransactionList();
    void on_addProductButton_clicked();
    void confirmAddProduct(QString,QString,QString);
    void confirmAddProvider(QString);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_clearButton_clicked();
    void on_addProviderButton_clicked();
    void on_sellPushButton_clicked();

private:
    Ui::MainWindow *ui;
    ProductTableModel *productModel;
    QSortFilterProxyModel *filterModel;
    ProviderTableModel *providerModel;
    QSqlTableModel *saleModel;
    QSqlRelationalTableModel *purchaseModel;
    int productIndex;
    int providerIndex;
    void showError(const QSqlError &err);
};

#endif // MAINWINDOW_H
