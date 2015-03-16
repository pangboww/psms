#ifndef PROVIDERTABLEMODEL_H
#define PROVIDERTABLEMODEL_H

#include <QtWidgets>
#include <QtSql>

class ProviderTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    ProviderTableModel(QWidget *parent = 0);
    ~ProviderTableModel();
    void addProvider(QString name);
};

#endif // PROVIDERTABLEMODEL_H
