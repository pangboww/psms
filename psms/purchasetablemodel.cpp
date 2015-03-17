#include "purchasetablemodel.h"

PurchaseTableModel::PurchaseTableModel(QWidget *parent):
    QSqlRelationalTableModel(parent)
{
    setEditStrategy(OnManualSubmit);
}

PurchaseTableModel::~PurchaseTableModel()
{

}

void PurchaseTableModel::addPurchaseRecord(int productID, int providerID, int amount){
    database().transaction();
    int r =rowCount();
    if(!insertRows(r, 1)) {
           qDebug() << "insertPurchaseRecord:" << lastError().text();
           return;
    }

    QDateTime now = QDateTime::currentDateTime();
    setData(index(r, 1), amount);
    setData(index(r, 2), now.toString("yyyy-MM-dd HH:mm:ss"));
    setData(index(r, 3), productID);
    setData(index(r, 4), providerID);
    qDebug() << "provider:"<<providerID;

    if(submitAll()) {
        bool y = database().commit();
        if(y) qDebug()<<"database insert successfully";
    } else {
        database().rollback();
        qDebug() << "Database Write Error" <<
                    "The database reported an error: " <<
                    lastError().text();
    }
}

