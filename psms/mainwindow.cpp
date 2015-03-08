#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "initdb.h"

#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(this, "Unable to load database", "This demo needs the SQLITE driver");

    QSqlError err = initDb();
    if (err.type() != QSqlError::NoError) {
        showError(err);
        return;
    }

    productModel = new QSqlTableModel(ui->productList);
    productModel->setTable("products");
    productModel->select();
    productModel->setHeaderData(0, Qt::Horizontal, tr("Name"));

    ui->productList->setModel(productModel);
    ui->productList->setModelColumn(1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}
