#include "providertablemodel.h"

ProviderTableModel::ProviderTableModel(QWidget *parent):
    QSqlTableModel(parent)
{
    setEditStrategy(OnManualSubmit);
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
        bool c = database().commit();
        if (c) qDebug()<<"Insert provider successfully!";
    } else {
        database().rollback();
        qDebug() << "Database Write Error" <<
                    "The database reported an error: " <<
                    lastError().text();
    }
}
