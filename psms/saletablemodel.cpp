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

int SaleTableModel::getTotalSaleOf(dateRange range){
    int r = rowCount();
    int totalSale = 0;
    QDateTime end = QDateTime::currentDateTime();
    QDateTime begin;
    switch (range) {
    case lastOneWeek:
        begin = end.addDays(-7);
        end = QDateTime::currentDateTime();
        for(int i = 0; i<r; i++){
            QDateTime dt = data(index(i, 2)).toDateTime();
            if(dt>=begin && dt<=end){
                totalSale += data(index(i, 1)).toInt();
            }
        }
        return totalSale;
        break;
    case lastOneMonth:
        begin = end.addMonths(-1);
        end = QDateTime::currentDateTime();
        for(int i = 0; i<r; i++){
            QDateTime dt = data(index(i, 2)).toDateTime();
            if(dt>=begin && dt<=end){
                totalSale += data(index(i, 1)).toInt();
            }
        }
        return totalSale;
        break;
    case lastOneYear:
        begin = end.addYears(-1);
        end = QDateTime::currentDateTime();
        for(int i = 0; i<r; i++){
            QDateTime dt = data(index(i, 2)).toDateTime();
            if(dt>=begin && dt<=end){
                totalSale += data(index(i, 1)).toInt();
            }
        }
        return totalSale;
        break;
    default:
        return 0;
        break;
    }
}

QVector<double> SaleTableModel::getSalesOf(dateRange range){
    int r = rowCount();
    QDateTime timeIndex = QDateTime::currentDateTime();
    QVector<double> sales;
    switch (range) {
    case lastTwoWeeks:
        for(int i=0; i<14; i++){
            timeIndex = timeIndex.addDays(-i);
            double sale = 0;
            for(int j = 0; j<r; j++){
                QDateTime dt = data(index(j,2)).toDateTime();
                if(dt.date().day() == timeIndex.date().day()){
                    sale += data(index(j, 1)).toDouble();
                }
            }
            sales << sale;
        }
        return sales;
        break;
    case lastOneYear:
        for(int i=0; i<12; i++){
            timeIndex = timeIndex.addMonths(-i);
            double sale = 0;
            for(int j = 0; j<r; j++){
                QDateTime dt = data(index(j,2)).toDateTime();
                if(dt.date().month() == timeIndex.date().month()){
                    sale += data(index(j, 1)).toDouble();
                }
            }
            sales << sale;
        }
        return sales;
        break;
    default:
        return sales;
        break;
    }
}

