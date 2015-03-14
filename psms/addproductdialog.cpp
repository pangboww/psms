#include "addproductdialog.h"
#include "ui_addproductdialog.h"

AddProductDialog::AddProductDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProductDialog)
{
    ui->setupUi(this);
}

AddProductDialog::~AddProductDialog()
{
    delete ui;
}

void AddProductDialog::accept(){
    emit addProduct(ui->titleEdit->text(),
                    ui->priceEdit->text(),
                    ui->stockEdit->text());
    this->hide();
}
