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
