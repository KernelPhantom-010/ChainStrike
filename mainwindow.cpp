#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <fstream>
#include <QInputDialog>
#include <QString>
#include <QListView>
#include <QStringListModel>
#include <capstone/capstone.h>
std::vector<QString> routesCreated;
QString log_beg = "[CNSE_LOG] ";
void MainWindow::routeAdding(){

    bool ok, ok2, illegalReg = false;
    QString enteredText = QInputDialog::getText(this, "Add a new Route", "Enter the Route-Name", QLineEdit::Normal, "", &ok);
    QString whichRegister = QInputDialog::getText(this, "Add a new Route", "Enter the Register you want to change/view\nExample inputs: rax, rcx, rdx, rsi, rdi,\nr8-r11", QLineEdit::Normal, "", &ok2);
    std::vector<std::string> allowedRegisters = {"rax", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10", "r11"};
    for (int initt = 0; initt < allowedRegisters.size(); initt++){
        std::string checkit = whichRegister.toStdString();
        if (checkit.compare(allowedRegisters[initt]) != 0){
            illegalReg = true;
        }else if (checkit.compare(allowedRegisters[initt]) == 0){
            illegalReg = false;
            break;
        }
    }
    if (illegalReg == true){            ui->plainTextEdit->appendPlainText("[CNSE_LOG] Illegal register entered. Aborting..");
        return;}

    if (ok == true && !enteredText.isEmpty()){
        ui->plainTextEdit->appendPlainText("[CNSE_LOG] New Route ['" + enteredText + "'] added.");
        ui->plainTextEdit->appendPlainText("[CNSE_LOG] Searching for register:" + whichRegister + " in binary..");
        routesCreated.push_back(enteredText);
    }

}


void MainWindow::binaryLoading(){

    QString fileName = QFileDialog::getOpenFileName(nullptr, "Select Binary to analyze", QDir::currentPath(), "All Files(*);; Executable Files (*.exe)");
    if (!fileName.isEmpty()){
        ui->plainTextEdit->appendPlainText(log_beg + "File provided for analysis -> ['" + fileName + "']..");
    }else{
        ui->plainTextEdit->appendPlainText(log_beg + "Something went wrong while trying to load the file..");
        return;
    }

    //hier mit Capstone arbeiten und disassembly code zeile für zeile in 'listView' eintragen.

    csh disasHandle;
    cs_insn * ins;
    size_t count = 0;

    //open the file in binary mode

    std::ifstream fileStreamm(fileName.toStdString().c_str(), std::ios::in | std::ios::binary);
    if (!fileStreamm){ui->plainTextEdit->appendPlainText(log_beg + "Couldn't open the file properly."); return;}
    fileStreamm.seekg(0, std::ios::end);
    std::streamsize size = fileStreamm.tellg();
    fileStreamm.seekg(0, std::ios::beg);
    std::vector<char> fileContent(size);
    if (fileStreamm.read(fileContent.data(), size)){
        ui->plainTextEdit->appendPlainText(log_beg + "Successfully wrote the file content into memory.");
    }
    fileStreamm.close();
    if (fileContent.size() < sizeof(IMAGE_DOS_HEADER)) {
        ui->plainTextEdit->appendPlainText("Invalid file: too small.");
        return;
    }
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &disasHandle) != CS_ERR_OK){
        ui->plainTextEdit->appendPlainText(log_beg + "Couldn't open the file in Capstone..");
        return;
    }

    auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(fileContent.data());
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        ui->plainTextEdit->appendPlainText("Invalid DOS signature.");
        return;
    }if (dosHeader->e_lfanew < 0 ||
        static_cast<size_t>(dosHeader->e_lfanew) >
            fileContent.size() - sizeof(IMAGE_NT_HEADERS64)) {
        ui->plainTextEdit->appendPlainText("Invalid PE header offset.");
        return;
    }

    auto ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS64>(fileContent.data() + dosHeader->e_lfanew);
    if (ntHeader->Signature != IMAGE_NT_SIGNATURE) {
        ui->plainTextEdit->appendPlainText("Invalid PE signature.");
        return;
    }
    auto section = IMAGE_FIRST_SECTION(ntHeader);
    QStringListModel* model = new QStringListModel(this);
    ui->listView->setModel(model);
    for (int i = 0; i < ntHeader->FileHeader.NumberOfSections; ++i, section++){
        if (strcmp(reinterpret_cast<const char*>(section->Name), ".text") == 0){
            const uint8_t* code = reinterpret_cast<const uint8_t*>(fileContent.data() + section->PointerToRawData);
            size_t codeSize = section->SizeOfRawData;
            size_t count = cs_disasm(disasHandle, code, codeSize, ntHeader->OptionalHeader.ImageBase + section->VirtualAddress, 0, &ins);
            if (count > 0){
                size_t j;
                ui->plainTextEdit->appendPlainText(log_beg + "Successfully read bytes! Printing in hex..");

            }else{
                ui->plainTextEdit->appendPlainText(log_beg + "Error -> failed to disassemble given code!");
                cs_close(&disasHandle);
                return;
            }
            QStringList items;

            for (size_t j = 0; j < count; j++)
            {
                items.append(QString::asprintf(
                    "0x%" PRIx64 ":\t%s\t\t%s",
                    ins[j].address,
                    ins[j].mnemonic,
                    ins[j].op_str
                    ));
            }

            model->setStringList(items);

        }
    }
    cs_free(ins, count);
    cs_close(&disasHandle);
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


    ui->listView->setStyleSheet(
        "QListView {"
        "   background-color: white;"
        "   border-radius: 10px;"
        "   padding: 6px;"
        "   color: black;"
        "}"
        "QListView::item:selected {"
        "   background-color: #252a40;"
        "   color: white;"
        "}"
        "QListView::item:hover {"
        "   background-color: #e0e4f0;"
        "}"
        );
    for (QListView* lv : {ui->listView, ui->listView_2, ui->listView_3}) {
        lv->setFrameShape(QFrame::NoFrame);
        lv->setAttribute(Qt::WA_StyledBackground, true);
        lv->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
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
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::binaryLoading);

}

MainWindow::~MainWindow()
{
    delete ui;
}