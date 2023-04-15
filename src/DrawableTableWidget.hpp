#pragma once

#include <QTableWidget>
#include <QMouseEvent>
#include <QHeaderView>


// Inherit from QTableWidget and override mouse events to allow drawing instead of only toggling
// single cells
class DrawableTableWidget : public QTableWidget {
    Q_OBJECT

public:
    explicit DrawableTableWidget(QWidget* parent = nullptr);

signals:
    void cellToggled(int row, int column);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool mouseIsPressed_ = false;
    int lastToggledRow_ = -1;
    int lastToggledColumn_ = -1;
};
