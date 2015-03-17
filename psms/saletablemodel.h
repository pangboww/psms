#ifndef SALETABLEMODEL_H
#define SALETABLEMODEL_H

#include <QWidget>
#include <QtSql>

class SaleTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    SaleTableModel(QWidget *parent = 0);
    ~SaleTableModel();
    void addSaleRecord(int productID,int amount);
};

#endif // SALETABLEMODEL_H


#ifndef PRODUCTTABLEMODEL_H
#define PRODUCTTABLEMODEL_H

#include <QtWidgets>
#include <QtSql>


class ProductTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    ProductTableModel(QWidget *parent = 0);
    ~ProductTableModel();
    void addProduct(QString title, QString price, QString stock);
    void searchProduct(QString *name);
    bool sell(int amount, int productIndex);
    void buy(int amount, int productIndex);
};

#endif // PRODUCTTABLEMODEL_H
