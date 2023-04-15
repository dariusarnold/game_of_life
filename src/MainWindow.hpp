#pragma once

#include <QMainWindow>

class DrawableTableWidget;
class QPushButton;
class QSlider;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() = default;

private:
    void initGrid(int width, int height);
    void updateGrid();
    void showSettingsDialog();
    void clearGrid();

    DrawableTableWidget* grid_;
    QTimer* timer_;
    std::vector<std::vector<int>> cells_;
    std::vector<std::vector<int>> next_cells_;
    QPushButton* playPauseButton_;
    QPushButton* stepButton_;
    QPushButton* clearButton_;
    QSlider* speedSlider_;
    QLabel* speedLabel_;
    bool isPlaying_ = true;

    int gridWidth_ = 50;
    int gridHeight_ = 34;

private slots:
    void toggleCellState(int row, int column);
};