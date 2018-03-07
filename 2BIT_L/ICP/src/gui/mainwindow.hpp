#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void clear_Blocks();

    void on_AddBlock_clicked();
    void on_SubBlock_clicked();

    void on_Scheme_clicked();

private:
    enum BlockType
    {
        BT_NONE = -1,
        BT_ADD,
        BT_SUB,
        BT_MUL,
        BT_DIV
    };

    Ui::MainWindow *ui;

    BlockType m_blockType = BT_NONE;
    QPushButton *m_selected = nullptr;
};

#endif // MAINWINDOW_HPP
