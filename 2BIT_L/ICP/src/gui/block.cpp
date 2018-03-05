#include "block.hpp"
#include "ui_block.h"

Block::Block(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Block)
{
    ui->setupUi(this);
}

Block::~Block()
{
    delete ui;
}
