#include "scheme.hpp"
#include "ui_scheme.h"

Scheme::Scheme(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

Scheme::~Scheme() {}

void Scheme::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
