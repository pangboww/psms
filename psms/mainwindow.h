#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void productIndexChanged();
private slots:
    void handleSelectionChanged(const QItemSelection&);
    void focusToProduct();
    void refreshTransactionList();
private:
    Ui::MainWindow *ui;
    QSqlTableModel *productModel;
    QSqlTableModel *transactionModel;
    int productIndex;
    void showError(const QSqlError &err);
};

#endif // MAINWINDOW_H
