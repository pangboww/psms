#include "saletablemodel.h"

SaleTableModel::SaleTableModel(QWidget *parent):
    QSqlTableModel(parent)
{
    setEditStrategy(OnManualSubmit);
}

SaleTableModel::~SaleTableModel()
{

}

void SaleTableModel::addSaleRecord(int productID, int amount){
    database().transaction();
    int r =rowCount();
    if(!insertRows(r, 1)) {
           qDebug() << "insertProduct:" << lastError().text();
           return;
    }

    QDateTime now = QDateTime::currentDateTime();
    setData(index(r, 1), amount);
    setData(index(r, 2), now.toString("yyyy-MM-dd HH:mm:ss"));
    setData(index(r, 3), productID);

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
