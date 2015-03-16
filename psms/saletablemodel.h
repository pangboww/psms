#ifndef SALETABLEMODEL_H
#define SALETABLEMODEL_H

#include <QWidget>

class SaleTableModel : public QSqlTableModel
{
public:
    SaleTableModel();
    ~SaleTableModel();
};

#endif // SALETABLEMODEL_H
