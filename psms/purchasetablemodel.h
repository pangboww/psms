#ifndef PURCHASETABLEMODEL_H
#define PURCHASETABLEMODEL_H

#include <QWidget>
#include <QtSql>

class PurchaseTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    PurchaseTableModel(QWidget *parent = 0);
    ~PurchaseTableModel();
    void addPurchaseRecord(int productID, int providerID, int amount);
};

#endif // PURCHASETABLEMODEL_H
