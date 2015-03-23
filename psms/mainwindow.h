#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>
#include <qcustomplot.h>
#include <producttablemodel.h>
#include <addproductdialog.h>
#include <addproviderdialog.h>
#include <providertablemodel.h>
#include <saletablemodel.h>
#include <purchasetablemodel.h>

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

    void on_buyPushButton_clicked();

private:
    Ui::MainWindow *ui;
    ProductTableModel *productModel;
    QSortFilterProxyModel *filterModel;
    ProviderTableModel *providerModel;
    SaleTableModel *saleModel;
    PurchaseTableModel *purchaseModel;
    int productIndex;
    int providerIndex;
    void showError(const QSqlError &err);

    QVector<double> lastTwoWeeksSale;
    QVector<double> lastOneYearSale;
    void setUpPlot();
};

#endif // MAINWINDOW_H
