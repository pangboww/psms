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

    productModel = new QSqlTableModel(ui->productListView);
    productModel->setTable("products");
    productModel->select();
    productModel->setHeaderData(0, Qt::Horizontal, tr("Name"));

    if (!productModel->select()) {
        showError(productModel->lastError());
        return;
    }

    ui->productListView->setModel(productModel);
    ui->productListView->setModelColumn(1);

    transactionModel = new QSqlTableModel(ui->transactionTableView);
    transactionModel->setTable("transactions");
    transactionModel->select();
    transactionModel->setFilter("id_product = 0");
    transactionModel->setHeaderData(1, Qt::Horizontal, tr("Amount"));
    transactionModel->setHeaderData(2,Qt::Horizontal, tr("Time"));

    if (!transactionModel->select()) {
        showError(transactionModel->lastError());
        return;
    }

    ui->transactionTableView->setModel(transactionModel);
    ui->transactionTableView->hideColumn(0);
    ui->transactionTableView->hideColumn(3);

    connect(ui->productListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(handleSelectionChanged(QItemSelection)));
    connect(this, SIGNAL(productIndexChanged()),
            this, SLOT(focusToProduct()));
    connect(this, SIGNAL(productIndexChanged()),
            this, SLOT(refreshTransactionList()));

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

void MainWindow::refreshTransactionList(){
    QString f = QString("id_product = %1").arg(productIndex);
    transactionModel->setFilter(f);
}


void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}
