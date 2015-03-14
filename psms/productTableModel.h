#ifndef PRODUCTTABLEMODEL_H
#define PRODUCTTABLEMODEL_H

#include <QtWidgets>
#include <QtSql>


class ProductTableModel : public QSqlTableModel
{
    Q_OBJECT


public:
    ProductTableModel(QWidget *parent = 0);
    ~ProductTableModel();
    void addProduct(QString *name, float price, int stock);
    void searchProduct(QString *name);
};

#endif // PRODUCTTABLEMODEL_H
