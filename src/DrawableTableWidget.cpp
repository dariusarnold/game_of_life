#include "DrawableTableWidget.hpp"

DrawableTableWidget::DrawableTableWidget(QWidget* parent)  : QTableWidget(parent) {
    // Hide column/row headers
    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);
}

void DrawableTableWidget::mousePressEvent(QMouseEvent* event)  {
    if (event->button() == Qt::LeftButton) {
        auto item = itemAt(event->pos());
        if (item) {
            lastToggledRow_ = item->row();
            lastToggledColumn_ = item->column();
            emit cellToggled(lastToggledRow_, lastToggledColumn_);
            mouseIsPressed_ = true;
        }
    }
}

void DrawableTableWidget::mouseMoveEvent(QMouseEvent* event) {
    if (mouseIsPressed_) {
        auto item = itemAt(event->pos());
        if (item) {
            int row = item->row();
            int column = item->column();
            // Store last toggled cell to prevent emitting the same signal multiple times for
            // the same cell, which leads to dragging within the same cell toggling the cell on
            // every mouse move instead of just on cell entry.
            if (row != lastToggledRow_ || column != lastToggledColumn_) {
                emit cellToggled(row, column);
                lastToggledRow_ = row;
                lastToggledColumn_ = column;
            }
        }
    }
}

void DrawableTableWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        mouseIsPressed_ = false;
    }
}