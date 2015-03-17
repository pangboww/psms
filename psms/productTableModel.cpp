#include "producttablemodel.h"

ProductTableModel::ProductTableModel(QWidget *parent):
    QSqlTableModel(parent)
{

}

ProductTableModel::~ProductTableModel()
{

}

void ProductTableModel::addProduct(QString title, QString price, QString stock){

    database().transaction();
    int r =rowCount();
    if(!insertRows(r, 1)) {
           qDebug() << "insertProduct:" << lastError().text();
           return;
    }
    qDebug() << rowCount() << "\n";

    setData(index(r, 1), title);
    setData(index(r, 2), price);
    setData(index(r, 3), stock);

    if(submitAll()) {
        database().commit();
    } else {
        database().rollback();
        qDebug() << "Database Write Error" <<
                    "The database reported an error: " <<
                    lastError().text();
    }
}

bool ProductTableModel::sell(int amount, int productIndex){
    database().transaction();
    int stock = data(index(productIndex, 3)).toInt();
    if (amount > stock) return false;
    stock -= amount;
    setData(index(productIndex,3), stock);
    if(submitAll()) {
        database().commit();
        return true;
    } else {
        database().rollback();
        qDebug() << "Database Write Error" <<
                    "The database reported an error: " <<
                    lastError().text();
        return false;
    }

}

void ProductTableModel::buy(int amount, int productID){
    database().transaction();
    int stock = data(index(productID, 3)).toInt();
    stock += amount;
    setData(index(productID,3), stock);
    if(submitAll()) {
        database().commit();
    } else {
        database().rollback();
        qDebug() << "Database Write Error" <<
                    "The database reported an error: " <<
                    lastError().text();
    }

}


