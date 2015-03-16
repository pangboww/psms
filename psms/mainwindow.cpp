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

    //Product View
    productIndex = 0;
    productModel = new ProductTableModel(ui->productListView);
    productModel->setTable("products");
    if (!productModel->select()) {
        showError(productModel->lastError());
        return;
    }
    filterModel = new QSortFilterProxyModel(this);
    filterModel->setSourceModel(productModel);
    filterModel->setFilterKeyColumn(1);
    ui->productListView->setModel(filterModel);
    ui->productListView->setModelColumn(1);

    //Sale Record View
    saleModel = new QSqlTableModel(ui->saleTableView);
    saleModel->setTable("sales");
    saleModel->setSort(2, Qt::DescendingOrder);
    saleModel->setHeaderData(1, Qt::Horizontal, tr("Amount"));
    saleModel->setHeaderData(2,Qt::Horizontal, tr("Time"));

    if (!saleModel->select()) {
        showError(saleModel->lastError());
        return;
    }
    ui->saleTableView->setModel(saleModel);
    ui->saleTableView->hideColumn(0);
    ui->saleTableView->hideColumn(3);

    purchaseModel = new QSqlRelationalTableModel(ui->purchaseTableView);
    purchaseModel->setTable("purchases");
    purchaseModel->setRelation(4, QSqlRelation("providers","id","name"));
    purchaseModel->sort(2, Qt::DescendingOrder);
    purchaseModel->setHeaderData(1, Qt::Horizontal, tr("Amount"));
    purchaseModel->setHeaderData(2, Qt::Horizontal, tr("Time"));
    purchaseModel->setHeaderData(4, Qt::Horizontal, tr("Provider"));
    if (!purchaseModel->select()) {
        showError(purchaseModel->lastError());
        return;
    }
    ui->purchaseTableView->setModel(purchaseModel);
    ui->purchaseTableView->hideColumn(0);
    ui->purchaseTableView->hideColumn(3);


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
    saleModel->setFilter(f);
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
        QVariant v = saleModel->record(3).value(0);
        qDebug() << v;
        v = saleModel->record(3).value(1);
        qDebug() << v;
        v = saleModel->record(3).value(2);
        qDebug() << v;
        v = saleModel->record(3).value(3);
        qDebug() << v;
        QSqlRecord record = saleModel->record();
        saleModel->insertRecord(1,record);
//        QVariant index = productModel->record(productIndex).value(0);
//        QSqlRecord record = saleModel->record();
//        record.setValue(1, QVariant(minus.toInt()));
//        record.setValue(2, QVariant(QDateTime::currentDateTime()));
//        record.setValue(3, index);
//        saleModel->insertRecord(1,record);
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
    productModel->selectRow(productModel->rowCount());
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QString s = QString(".?%1.?").arg(arg1);
    QRegExp regExp = QRegExp(s);
    regExp.setCaseSensitivity(Qt::CaseInsensitive);
    filterModel->setFilterRegExp(regExp);
}
