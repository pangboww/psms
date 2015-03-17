#include "purchasetablemodel.h"

PurchaseTableModel::PurchaseTableModel(QWidget *parent):
    QSqlRelationalTableModel(parent)
{

}

PurchaseTableModel::~PurchaseTableModel()
{

}

void PurchaseTableModel::addPurchaseRecord(int productID, int providerID, int amount){

}

