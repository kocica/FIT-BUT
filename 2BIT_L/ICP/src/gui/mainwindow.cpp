#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear_Blocks()
{
    ui->AddBlock->setStyleSheet("background-color: rgb(225, 225, 225)");
    ui->SubBlock->setStyleSheet("background-color: rgb(225, 225, 225)");
}

void MainWindow::on_AddBlock_clicked()
{
    this->clear_Blocks();
    ui->AddBlock->setStyleSheet("background-color: green");
    this->m_selected = ui->AddBlock;
    this->m_blockType = this->BT_ADD;
}

void MainWindow::on_SubBlock_clicked()
{
    this->clear_Blocks();
    ui->SubBlock->setStyleSheet("background-color: green");
    this->m_selected = ui->SubBlock;
    this->m_blockType = this->BT_SUB;
}

void MainWindow::on_Scheme_clicked()
{
    // Care the positions of new block

    if (this->m_selected != nullptr)
    {
        this->m_selected->setStyleSheet("background-color: rgb(225, 225, 225)");

        QPushButton *block = new QPushButton(ui->Scheme);
        block->resize(71, 71);
        block->move(block->parentWidget()->mapFromGlobal(QCursor::pos()));
        block->setStyleSheet("font-size: 21px");

        switch(this->m_blockType)
        {
        case BT_ADD:
            block->setText("+");
            break;
        case BT_SUB:
            block->setText("-");
            break;
        }
        block->show();

        this->m_selected = nullptr;
        this->m_blockType = this->BT_NONE;
    }
}

