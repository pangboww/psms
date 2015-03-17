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


