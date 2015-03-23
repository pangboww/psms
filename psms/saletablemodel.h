#ifndef SALETABLEMODEL_H
#define SALETABLEMODEL_H

#include <QWidget>
#include <QtSql>

class SaleTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    enum dateRange {lastOneWeek, lastTwoWeeks, lastOneMonth, lastOneYear};
    SaleTableModel(QWidget *parent = 0);
    ~SaleTableModel();
    void addSaleRecord(int productID,int amount);
    int getTotalSaleOf(dateRange range);
    QVector<double> getSalesOf(dateRange range);
};



#endif // SALETABLEMODEL_H


