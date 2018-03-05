#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <QDialog>

namespace Ui {
class Block;
}

class Block : public QDialog
{
    Q_OBJECT

public:
    explicit Block(QWidget *parent = 0);
    ~Block();

private:
    Ui::Block *ui;
};

#endif // BLOCK_HPP
