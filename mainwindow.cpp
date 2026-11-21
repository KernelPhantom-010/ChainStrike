#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- Sidebar ---
    ui->widget->setStyleSheet("background: black; border-radius: 10px;");
    ui->widget->setMinimumWidth(171);
    ui->widget->setMaximumWidth(171);
    ui->widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    ui->pushButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->pushButton_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->pushButton_3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(ui->widget);
    sidebarLayout->setContentsMargins(12, 12, 12, 12);
    sidebarLayout->setSpacing(8);
    sidebarLayout->addWidget(ui->pushButton);
    sidebarLayout->addWidget(ui->pushButton_2);
    sidebarLayout->addWidget(ui->pushButton_3);
    sidebarLayout->addStretch(1);

    QString listStyle = "QListView { border-radius: 10px; background-color: white; padding: 6px; }";
    for (QListView* lv : {ui->listView, ui->listView_2, ui->listView_3}) {
        lv->setFrameShape(QFrame::NoFrame);
        lv->setAttribute(Qt::WA_StyledBackground, true);
        lv->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        lv->setStyleSheet(listStyle);
    }

    QWidget *rightContainer = new QWidget(ui->centralwidget);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(8);

    rightLayout->addWidget(ui->label);
    rightLayout->addWidget(ui->listView, 1);

    QHBoxLayout *bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(8);
    bottomRow->addWidget(ui->listView_2, 1);
    bottomRow->addWidget(ui->listView_3, 1);
    rightLayout->addLayout(bottomRow, 2);


    QHBoxLayout *mainLayout = new QHBoxLayout(ui->centralwidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(ui->widget);
    mainLayout->addWidget(rightContainer, 1);



}

MainWindow::~MainWindow()
{
    delete ui;
}