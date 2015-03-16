#include "saletablemodel.h"

SaleTableModel::SaleTableModel(QWidget *parent):
    QSqlTableModel(parent)
{

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
    qDebug() << rowCount() << "\n";

    QDateTime now = QDateTime::currentDateTime();
    qDebug() << data(index(r,0));
    setData(index(r, 1), amount);
    setData(index(r, 2), now);
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

