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

    //Product View
    productIndex = 0;
    productModel = new ProductTableModel(ui->productListView);
    productModel->setTable("products");
    productModel->setSort(1, Qt::AscendingOrder);
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
    saleModel = new SaleTableModel(ui->saleTableView);
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
    ui->saleTableView->setColumnWidth(2,150);
    //Purchase Record View
    purchaseModel = new PurchaseTableModel(ui->purchaseTableView);
    purchaseModel->setTable("purchases");
    purchaseModel->setRelation(4, QSqlRelation("providers","id","name"));
    purchaseModel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
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
    ui->purchaseTableView->setColumnWidth(1,70);
    ui->purchaseTableView->setColumnWidth(2,150);

    //Provider Choose ComboBox
    providerModel = new ProviderTableModel(ui->providerBox);
    providerModel->setTable("providers");
    providerModel->sort(1, Qt::AscendingOrder);
    if (!providerModel->select()) {
        showError(providerModel->lastError());
        return;
    }
    ui->providerBox->setModel(providerModel);
    ui->providerBox->setModelColumn(1);




    connect(ui->productListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(handleSelectionChanged(QItemSelection)));
    connect(this, SIGNAL(productIndexChanged()),
            this, SLOT(focusToProduct()));
    connect(this, SIGNAL(productIndexChanged()),
            this, SLOT(refreshTransactionList()));
    connect(ui->providerBox,SIGNAL(currentIndexChanged(int)),
            this, SLOT(handleProviderChanged(int)));

    emit productIndexChanged();
    ui->providerBox->currentIndexChanged(0);

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

    QVariant sale = saleModel->getTotalSaleOf(SaleTableModel::lastOneWeek);
    ui->label_last_week->setText(sale.toString());
    sale = saleModel->getTotalSaleOf(SaleTableModel::lastOneMonth);
    ui->label_last_month->setText(sale.toString());
    sale = saleModel->getTotalSaleOf(SaleTableModel::lastOneYear);
    ui->label_last_year->setText(sale.toString());

    lastTwoWeeksSale = saleModel->getSalesOf(SaleTableModel::lastTwoWeeks);
    lastOneYearSale = saleModel->getSalesOf(SaleTableModel::lastOneYear);
    setUpPlot();
}

void MainWindow::refreshTransactionList(){
    int productID = productModel->itemData(productModel->index(productIndex, 0))[0].toInt();
    QString f = QString("id_product = %1").arg(productID);
    saleModel->setFilter(f);
    purchaseModel->setFilter(f);
}


void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
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
    productModel->select();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QString s = QString(".?%1.?").arg(arg1);
    QRegExp regExp = QRegExp(s);
    regExp.setCaseSensitivity(Qt::CaseInsensitive);
    filterModel->setFilterRegExp(regExp);
}

void MainWindow::on_clearButton_clicked()
{
    ui->lineEdit->setText("");
}

void MainWindow::on_addProviderButton_clicked()
{
    AddProviderDialog *dialog = new AddProviderDialog(this);
    dialog->show();
    connect(dialog,
            SIGNAL(addProvider(QString)),
            this,
            SLOT(confirmAddProvider(QString)));
}

void MainWindow::confirmAddProvider(QString name){
    providerModel->addProvider(name);
    providerModel->select();
    ui->providerBox->setCurrentIndex(0);
    purchaseModel->select();
}

void MainWindow::handleProviderChanged(int i){
    providerIndex = providerModel->itemData(providerModel->index(i, 0))[0].toInt();
    qDebug() << providerIndex;
}

void MainWindow::on_sellPushButton_clicked()
{
    if(!ui->sellEdit->text().isEmpty()){
        int amount =  ui->sellEdit->text().toInt();
        int productID = productModel->itemData(productModel->index(productIndex, 0))[0].toInt();
        if(amount == 0)return;
        saleModel->addSaleRecord(productID,amount);
        refreshTransactionList();
        productModel->sell(amount, productIndex);
        focusToProduct();
    }
}

void MainWindow::on_buyPushButton_clicked()
{
    if(!ui->buyEdit->text().isEmpty()){
        int amount =  ui->buyEdit->text().toInt();
        if(amount == 0)return;
        int productID = productModel->itemData(productModel->index(productIndex, 0))[0].toInt();
        purchaseModel->addPurchaseRecord(productID,providerIndex,amount);
        refreshTransactionList();
        productModel->buy(amount, productIndex);
        focusToProduct();
    }
}

void MainWindow::setUpPlot(){
    ui->two_weeks_sale_plot->clearPlottables();
    ui->one_year_sale_plot->clearPlottables();

    QCPBars *twoWeeks = new QCPBars(ui->two_weeks_sale_plot->xAxis, ui->two_weeks_sale_plot->yAxis);
    QCPBars *oneYear = new QCPBars(ui->one_year_sale_plot->xAxis, ui->one_year_sale_plot->yAxis);
    ui->two_weeks_sale_plot->addPlottable(twoWeeks);
    ui->one_year_sale_plot->addPlottable(oneYear);
    QPen pen;
    pen.setWidthF(1.2);

    twoWeeks->setName("Two Weeks sales");
    oneYear->setName("One Year Sales");
    pen.setColor(QColor(150, 222, 0));
    twoWeeks->setPen(pen);
    twoWeeks->setBrush(QColor(150, 222, 0, 70));

    QVector<double> tick1;
    QVector<double> tick2;
    tick1 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14;
    tick2 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
    ui->two_weeks_sale_plot->xAxis->setAutoTicks(true);
    ui->two_weeks_sale_plot->xAxis->setAutoTickLabels(true);
    ui->two_weeks_sale_plot->xAxis->setTickVector(tick1);
    ui->two_weeks_sale_plot->xAxis->setTickLabelRotation(60);
    ui->two_weeks_sale_plot->xAxis->setSubTickCount(0);
    ui->two_weeks_sale_plot->xAxis->setTickLength(0, 4);
    ui->two_weeks_sale_plot->xAxis->grid()->setVisible(true);
    ui->two_weeks_sale_plot->xAxis->setRange(0, 15);

    ui->one_year_sale_plot->xAxis->setAutoTicks(false);
    ui->one_year_sale_plot->xAxis->setAutoTickLabels(true);
    ui->one_year_sale_plot->xAxis->setTickVector(tick2);
    ui->one_year_sale_plot->xAxis->setTickLabelRotation(60);
    ui->one_year_sale_plot->xAxis->setSubTickCount(0);
    ui->one_year_sale_plot->xAxis->setTickLength(0, 4);
    ui->one_year_sale_plot->xAxis->grid()->setVisible(true);
    ui->one_year_sale_plot->xAxis->setRange(0, 13);


    int max = 0;
    foreach (double i, lastTwoWeeksSale) {
        if(i > max) max = i;
    }

    ui->two_weeks_sale_plot->yAxis->setRange(0, max);
    ui->two_weeks_sale_plot->yAxis->setPadding(5);
    ui->two_weeks_sale_plot->yAxis->setLabel("Last Two Weeks Sales");
    ui->two_weeks_sale_plot->yAxis->grid()->setSubGridVisible(true);

    max = 0;
    foreach (double i, lastOneYearSale) {
        if(i > max) max = i;
    }
    ui->one_year_sale_plot->yAxis->setRange(0, max);
    ui->one_year_sale_plot->yAxis->setPadding(5);
    ui->one_year_sale_plot->yAxis->setLabel("Last One Year Sales");
    ui->one_year_sale_plot->yAxis->grid()->setSubGridVisible(true);

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->two_weeks_sale_plot->yAxis->grid()->setPen(gridPen);
    ui->one_year_sale_plot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->two_weeks_sale_plot->yAxis->grid()->setSubGridPen(gridPen);
    ui->one_year_sale_plot->yAxis->grid()->setSubGridPen(gridPen);
    twoWeeks->setData(tick1, lastTwoWeeksSale);
    oneYear->setData(tick2, lastOneYearSale);

    ui->one_year_sale_plot->replot();
    ui->two_weeks_sale_plot->replot();
}
