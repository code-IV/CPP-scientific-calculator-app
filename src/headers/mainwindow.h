#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QPushButton>
#include <QGridLayout>  // ADD THIS LINE

class QLineEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void digitClicked();
    void operatorClicked();
    void functionClicked();
    void equalClicked();
    void clearClicked();
    void pointClicked();
    void backspaceClicked();
    void memoryStoreClicked();
    void memoryRecallClicked();

private:
    void createButtons();
    void calculate();
    // REMOVE THIS LINE: void addButtonToGrid(QGridLayout *layout, QPushButton *button, int row, int col, int rowSpan = 1, int colSpan = 1);

    QLineEdit *display;
    QPushButton *createButton(const QString &text, const char *member);
    
    // Current calculation state
    QString pendingOperator;
    double resultSoFar;
    bool waitingForOperand;
    double memory;
};

#endif // MAINWINDOW_H