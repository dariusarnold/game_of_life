#include "MainWindow.hpp"

#include "DrawableTableWidget.hpp"
#include <QTimer>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMenu>
#include <QMenuBar>
#include <QDialog>
#include <QRandomGenerator>


MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent),
        grid_(new DrawableTableWidget(this)),
        timer_(new QTimer(this)),
        playPauseButton_(new QPushButton("Pause", this)),
        stepButton_(new QPushButton("Step", this)),
        clearButton_(new QPushButton("Clear", this)),
        speedSlider_(new QSlider(Qt::Horizontal, this)),
        speedLabel_(new QLabel("Speed: 100ms", this)) {
    setWindowTitle("Game of Life");
    resize(1024, 768); // Set default window size
    setCentralWidget(grid_);
    initGrid(gridWidth_, gridHeight_);

    timer_->setInterval(100);
    connect(timer_, &QTimer::timeout, this, &MainWindow::updateGrid);
    timer_->start();

    connect(playPauseButton_, &QPushButton::clicked, this, [this]() {
        if (isPlaying_) {
            timer_->stop();
            playPauseButton_->setText("Play");
            isPlaying_ = false;
        } else {
            timer_->start();
            playPauseButton_->setText("Pause");
            isPlaying_ = true;
        }
    });

    connect(grid_, &DrawableTableWidget::cellToggled, this, &MainWindow::toggleCellState);
    connect(grid_, &QTableWidget::cellClicked, this, &MainWindow::toggleCellState);

    connect(clearButton_, &QPushButton::clicked, this, &MainWindow::clearGrid);
    connect(stepButton_, &QPushButton::clicked, this, &MainWindow::updateGrid);


    grid_->setSelectionMode(QAbstractItemView::NoSelection);   // Disable cell selection highlight
    grid_->setEditTriggers(QAbstractItemView::NoEditTriggers); // Disable cell editing

    // Configure the speed slider
    speedSlider_->setMinimum(10);
    speedSlider_->setMaximum(1000);
    speedSlider_->setValue(100);
    connect(speedSlider_, &QSlider::valueChanged, this, [this](int value) {
        timer_->setInterval(value);
        speedLabel_->setText(QString("Speed: %1ms").arg(value));
    });

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(playPauseButton_);
    buttonLayout->addWidget(stepButton_);
    buttonLayout->addWidget(clearButton_);
    buttonLayout->addWidget(speedLabel_);
    buttonLayout->addWidget(speedSlider_);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(grid_);
    layout->addLayout(buttonLayout);

    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(layout);

    QMenuBar* menuBar = new QMenuBar(this);
    QMenu* settingsMenu = new QMenu("Settings", menuBar);
    QAction* gridSizeAction = new QAction("Grid Size", this);
    connect(gridSizeAction, &QAction::triggered, this, &MainWindow::showSettingsDialog);
    settingsMenu->addAction(gridSizeAction);
    menuBar->addMenu(settingsMenu);
    setMenuBar(menuBar);
}

void MainWindow::initGrid(int width, int height) {
    grid_->setRowCount(height);
    grid_->setColumnCount(width);

    QScreen* screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qWarning("Unable to get the primary screen.");
    }

    // Make the cells in the grid square
    grid_->horizontalHeader()->setMinimumSectionSize(0);
    grid_->verticalHeader()->setMinimumSectionSize(0);
    int cellSize = 20;

    cells_.resize(height);
    next_cells_.resize(height);
    for (int i = 0; i < height; ++i) {
        cells_[i].resize(width);
        next_cells_[i].resize(width);
        for (int j = 0; j < width; ++j) {
            cells_[i][j] =  QRandomGenerator::global()->bounded(2);;
            next_cells_[i][j] = 0;
            QTableWidgetItem* item = new QTableWidgetItem();
            if (cells_[i][j] == 0) {
                item->setBackground(Qt::black);
            } else {
                item->setBackground(Qt::white);
            }
            grid_->setItem(i, j, item);
            grid_->setColumnWidth(j, cellSize);
            grid_->setRowHeight(i, cellSize);
        }
    }
}

void MainWindow::clearGrid() {
    for (int i = 0; i < gridHeight_; ++i) {
        for (int j = 0; j < gridWidth_; ++j) {
            cells_[i][j] = 0;
            QTableWidgetItem* item = grid_->item(i, j);
            item->setBackground(Qt::black);
        }
    }
}

void MainWindow::updateGrid() {
    for (int i = 0; i < gridHeight_; ++i) {
        for (int j = 0; j < gridWidth_; ++j) {
            int count = 0;
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    int xi = (i + x + gridHeight_) % gridHeight_;
                    int yj = (j + y + gridWidth_) % gridWidth_;
                    if (cells_[xi][yj] == 1) {
                        count++;
                    }
                }
            }
            count -= cells_[i][j];
            if (cells_[i][j] == 1 && (count < 2 || count > 3)) {
                next_cells_[i][j] = 0;
            } else if (cells_[i][j] == 0 && count == 3) {
                next_cells_[i][j] = 1;
            } else {
                next_cells_[i][j] = cells_[i][j];
            }
        }
    }
    for (int i = 0; i < gridHeight_; ++i) {
        for (int j = 0; j < gridWidth_; ++j) {
            cells_[i][j] = next_cells_[i][j];
            QTableWidgetItem* item = grid_->item(i, j);
            if (cells_[i][j] == 0) {
                item->setBackground(Qt::black);
            } else {
                item->setBackground(Qt::white);
            }
        }
    }
}

void MainWindow::toggleCellState(int row, int column) {
    if (cells_[row][column] == 0) {
        cells_[row][column] = 1;
        QTableWidgetItem* item = grid_->item(row, column);
        item->setBackground(Qt::white);
    } else {
        cells_[row][column] = 0;
        QTableWidgetItem* item = grid_->item(row, column);
        item->setBackground(Qt::black);
    }
}

void MainWindow::showSettingsDialog() {
    QDialog settingsDialog(this);
    settingsDialog.setWindowTitle("Grid Size Settings");

    QSpinBox* widthSpinBox = new QSpinBox(&settingsDialog);
    widthSpinBox->setRange(10, 100);
    widthSpinBox->setValue(gridWidth_);

    QSpinBox* heightSpinBox = new QSpinBox(&settingsDialog);
    heightSpinBox->setRange(10, 100);
    heightSpinBox->setValue(gridHeight_);

    QDialogButtonBox* buttonBox
        = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &settingsDialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &settingsDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &settingsDialog, &QDialog::reject);

    QFormLayout* formLayout = new QFormLayout(&settingsDialog);
    formLayout->addRow("Width:", widthSpinBox);
    formLayout->addRow("Height:", heightSpinBox);
    formLayout->addWidget(buttonBox);

    if (settingsDialog.exec() == QDialog::Accepted) {
        gridWidth_ = widthSpinBox->value();
        gridHeight_ = heightSpinBox->value();
        initGrid(gridWidth_, gridHeight_);
    }
}