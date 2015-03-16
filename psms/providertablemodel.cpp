#include "providertablemodel.h"

ProviderTableModel::ProviderTableModel(QWidget *parent):
    QSqlTableModel(parent)
{

}

ProviderTableModel::~ProviderTableModel()
{

}

void ProviderTableModel::addProvider(QString name){

    database().transaction();
    int r =rowCount();
    if(!insertRows(r, 1)) return;

    setData(index(r, 1), name);
    if(submitAll()) {
        database().commit();
    } else {
        database().rollback();
        qDebug() << "Database Write Error" <<
                    "The database reported an error: " <<
                    lastError().text();
    }
}
