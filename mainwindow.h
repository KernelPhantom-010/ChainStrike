#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QModelIndexList>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void routeAdding();

private:
    Ui::MainWindow *ui;
    QDialog* routeAddingDialog;
};
#endif // MAINWINDOW_H
