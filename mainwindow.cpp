#include <windows.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QListView>

void MainWindow::routeAdding(){

    bool ok;
    QString enteredText = QInputDialog::getText(this, "Add a new Route", "Enter the Route-Name", QLineEdit::Normal, "", &ok);

    if (ok == true && !enteredText.isEmpty()){
        ui->plainTextEdit->appendPlainText("[CNSE_LOG] New Route " + enteredText + "added.");
    }

}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


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
        lv->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        lv->setStyleSheet(listStyle);
    }


    QWidget *rightContainer = new QWidget(ui->centralwidget);
    QVBoxLayout *rightMainLayout = new QVBoxLayout(rightContainer);
    rightMainLayout->setContentsMargins(8, 8, 8, 8);
    rightMainLayout->setSpacing(8);


    QHBoxLayout *topRow = new QHBoxLayout();


    QVBoxLayout *routesLayout = new QVBoxLayout();
    routesLayout->addWidget(ui->label_2);
    routesLayout->addWidget(ui->listView_2);


    QVBoxLayout *disasmLayout = new QVBoxLayout();
    disasmLayout->addWidget(ui->label);
    disasmLayout->addWidget(ui->listView);


    QVBoxLayout *logsLayout = new QVBoxLayout();
    logsLayout->addWidget(ui->label_3);
    logsLayout->addWidget(ui->plainTextEdit);


    topRow->addLayout(routesLayout, 1);
    topRow->addLayout(disasmLayout, 3);
    topRow->addLayout(logsLayout, 1);


    QHBoxLayout *bottomRow = new QHBoxLayout();


    QVBoxLayout *exploitLayout = new QVBoxLayout();
    exploitLayout->addWidget(ui->label_5);
    exploitLayout->addWidget(ui->plainTextEdit_2);


    QVBoxLayout *chainsLayout = new QVBoxLayout();
    chainsLayout->addWidget(ui->label_4);
    chainsLayout->addWidget(ui->listView_3);

    bottomRow->addLayout(exploitLayout, 1);
    bottomRow->addLayout(chainsLayout, 1);


    rightMainLayout->addLayout(topRow, 1);
    rightMainLayout->addLayout(bottomRow, 1);


    QHBoxLayout *mainLayout = new QHBoxLayout(ui->centralwidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(ui->widget);
    mainLayout->addWidget(rightContainer, 1);

    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::routeAdding);


}

MainWindow::~MainWindow()
{
    delete ui;
}