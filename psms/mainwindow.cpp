#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "initdb.h"

#include <QtSql>
#include <QModelIndex>
#include <QtDebug>
#include <QAbstractItemView>

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

    productIndex = 0;
    productModel = new QSqlTableModel(ui->productList);
    productModel->setTable("products");
    productModel->select();
    productModel->setHeaderData(0, Qt::Horizontal, tr("Name"));

    if (!productModel->select()) {
        showError(productModel->lastError());
        return;
    }

    ui->productList->setModel(productModel);
    ui->productList->setModelColumn(1);

    connect(ui->productList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(handleSelectionChanged(QItemSelection)));

    connect(this, SIGNAL(productIndexChanged()),
            this, SLOT(focusToProduct()));

    focusToProduct();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleSelectionChanged(const QItemSelection& selection){
    productIndex = selection.indexes().first().row();
    emit productIndexChanged();
}

void MainWindow::focusToProduct(){
    QVariant title = productModel->record(productIndex).value(1);
    QVariant price = productModel->record(productIndex).value(2);
    QVariant stock = productModel->record(productIndex).value(3);
    ui->label_title->setText(title.toString());
    ui->label_price->setText(price.toString());
    ui->label_stock->setText(stock.toString());
}

void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}
