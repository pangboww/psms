#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "initdb.h"

#include <QtSql>
#include <QModelIndex>
#include <QtDebug>
#include <QAbstractItemView>
#include <QDateTime>
//#include <QSqlRecord>

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

    productModel = new ProductTableModel(ui->productListView);
    productModel->setTable("products");
    if (!productModel->select()) {
        showError(productModel->lastError());
        return;
    }

    ui->productListView->setModel(productModel);
    ui->productListView->setModelColumn(1);

    transactionModel = new QSqlTableModel(ui->transactionTableView);
    transactionModel->setTable("transactions");
    transactionModel->setSort(2, Qt::DescendingOrder);
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

    emit productIndexChanged();
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
    QString f = QString("id_product = %1").arg(productIndex+1);
    transactionModel->setFilter(f);
}


void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}

void MainWindow::on_inputPushButton_clicked()
{
    QString add = ui->addInput->text();
    QString minus = ui->minusInput->text();

    if((!add.isEmpty())&&(!minus.isEmpty())){
        qDebug() << "Both inputs have value!";
        return;
    }
    if(!minus.isEmpty()){
        QVariant v = transactionModel->record(3).value(0);
        qDebug() << v;
        v = transactionModel->record(3).value(1);
        qDebug() << v;
        v = transactionModel->record(3).value(2);
        qDebug() << v;
        v = transactionModel->record(3).value(3);
        qDebug() << v;
        QSqlRecord record = transactionModel->record();
        transactionModel->insertRecord(1,record);
//        QVariant index = productModel->record(productIndex).value(0);
//        QSqlRecord record = transactionModel->record();
//        record.setValue(1, QVariant(minus.toInt()));
//        record.setValue(2, QVariant(QDateTime::currentDateTime()));
//        record.setValue(3, index);
//        transactionModel->insertRecord(1,record);
    }
}

void MainWindow::on_addProductButton_clicked()
{
    AddProductDialog *dialog = new AddProductDialog(this);
    dialog->show();
    connect(dialog,
            SIGNAL(addProduct(QString,QString,QString)),
            this,
            SLOT(confirmAddProduct(QString,QString,QString)));
}

void MainWindow::confirmAddProduct(QString title, QString price, QString stock){
    productModel->addProduct(title, price, stock);
}
