#ifndef SCHEME_HPP
#define SCHEME_HPP

#include <QLabel>
#include <QWidget>
#include <Qt>

namespace Ui {
class Scheme;
}


class Scheme : public QLabel {
    Q_OBJECT

public:
    explicit Scheme(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~Scheme();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif
