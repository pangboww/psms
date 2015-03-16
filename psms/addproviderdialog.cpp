#include "addproviderdialog.h"
#include "ui_addproviderdialog.h"

AddProviderDialog::AddProviderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProviderDialog)
{
    ui->setupUi(this);
}

AddProviderDialog::~AddProviderDialog()
{
    delete ui;
}

void AddProviderDialog::accept(){
    if(!ui->lineEdit->text().isEmpty()){
        emit addProvider(ui->lineEdit->text());
        this->hide();
    }
    return;
}

