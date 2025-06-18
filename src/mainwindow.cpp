#include "headers/mainwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <cmath>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), resultSoFar(0.0), waitingForOperand(true), memory(0.0)
{
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(25);
    display->setMinimumHeight(50);
    
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(display, 0, 0, 1, 5);
    
    // Create button rows
    QStringList buttonRows = {
        "MC,MR,⌫,CE,C",
        "sin,cos,tan,π,e",
        "7,8,9,/,√",
        "4,5,6,*,x²",
        "1,2,3,-,x^y",
        "0,.,=,+,"
    };

   for (int row = 0; row < buttonRows.size(); ++row) {
    QStringList buttons = buttonRows[row].split(",");
    for (int col = 0; col < buttons.size(); ++col) {
        if (!buttons[col].isEmpty()) {
            QString text = buttons[col];
            QPushButton *button = new QPushButton(text);
            button->setMinimumSize(50, 50);

            // Connect the button to the appropriate slot
            if (text == "=") {
                connect(button, &QPushButton::clicked, this, &MainWindow::equalClicked);
            } else if (text == "C" || text == "CE") {
                connect(button, &QPushButton::clicked, this, &MainWindow::clearClicked);
            } else if (text == "⌫") {
                connect(button, &QPushButton::clicked, this, &MainWindow::backspaceClicked);
            } else if (text == ".") {
                connect(button, &QPushButton::clicked, this, &MainWindow::pointClicked);
            } else if (text == "MC" || text == "MR") {
                // Fixed connection with if/else
                if (text == "MC") {
                    connect(button, &QPushButton::clicked, this, &MainWindow::memoryStoreClicked);
                } else {
                    connect(button, &QPushButton::clicked, this, &MainWindow::memoryRecallClicked);
                }
            } else if (text == "π" || text == "e" || 
                       text == "sin" || text == "cos" || 
                       text == "tan" || text == "√" || 
                       text == "x²" || text == "x^y") {
                connect(button, &QPushButton::clicked, this, &MainWindow::functionClicked);
            } else {
                // Digits and operators
                if (text >= "0" && text <= "9" || text == "00") {
                    connect(button, &QPushButton::clicked, this, &MainWindow::digitClicked);
                } else {
                    connect(button, &QPushButton::clicked, this, &MainWindow::operatorClicked);
                }
            }
            
            // Add to layout with column span for = button
            int colSpan = (text == "=") ? 2 : 1;
            mainLayout->addWidget(button, row + 1, col, 1, colSpan);
            
            // Skip next column for = button
            if (text == "=") col++;
        }
    }
}
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

QPushButton *MainWindow::createButton(const QString &text, const char *member) {
    QPushButton *button = new QPushButton(text);
    button->setMinimumSize(50, 50);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void MainWindow::digitClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QString digitValue = clickedButton->text();
    
    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }
    
    // Handle special buttons
    if (digitValue == "00") {
        if (display->text() != "0") {
            display->setText(display->text() + "00");
        }
    } 
    // Handle normal digits
    else {
        if (display->text() == "0" && digitValue != "00") {
            display->setText(digitValue);
        } else {
            display->setText(display->text() + digitValue);
        }
    }
}

void MainWindow::operatorClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    
    if (!pendingOperator.isEmpty()) {
        calculate();
        display->setText(QString::number(resultSoFar));
    } else {
        resultSoFar = operand;
    }
    
    pendingOperator = clickedOperator;
    waitingForOperand = true;
}

void MainWindow::functionClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QString function = clickedButton->text();
    double value = display->text().toDouble();
    double result = 0.0;
    
    if (function == "sin") result = sin(value * M_PI / 180.0);
    else if (function == "cos") result = cos(value * M_PI / 180.0);
    else if (function == "tan") result = tan(value * M_PI / 180.0);
    else if (function == "√") result = sqrt(value);
    else if (function == "x²") result = value * value;
    else if (function == "π") result = M_PI;
    else if (function == "e") result = M_E;
    else if (function == "x^y") {
        // Handled as operator
        operatorClicked();
        return;
    }
    
    display->setText(QString::number(result));
    waitingForOperand = true;
}

void MainWindow::pointClicked() {
    if (waitingForOperand) {
        display->setText("0.");
    } else if (!display->text().contains('.')) {
        display->setText(display->text() + ".");
    }
    waitingForOperand = false;
}

void MainWindow::backspaceClicked() {
    QString text = display->text();
    if (text.length() > 0) {
        text.chop(1);
        if (text.isEmpty()) {
            text = "0";
            waitingForOperand = true;
        }
        display->setText(text);
    }
}

void MainWindow::clearClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (clickedButton->text() == "C") {
        resultSoFar = 0.0;
        pendingOperator.clear();
    }
    display->setText("0");
    waitingForOperand = true;
}

void MainWindow::memoryStoreClicked() {
    memory = display->text().toDouble();
}

void MainWindow::memoryRecallClicked() {
    display->setText(QString::number(memory));
    waitingForOperand = true;
}

void MainWindow::equalClicked() {
    calculate();
    pendingOperator.clear();
    waitingForOperand = true;
}

void MainWindow::calculate() {
    double operand = display->text().toDouble();
    
    if (pendingOperator == "+") {
        resultSoFar += operand;
    } else if (pendingOperator == "-") {
        resultSoFar -= operand;
    } else if (pendingOperator == "*") {
        resultSoFar *= operand;
    } else if (pendingOperator == "/") {
        if (operand == 0.0) {
            display->setText("Error: Div by 0");
            return;
        }
        resultSoFar /= operand;
    } else if (pendingOperator == "x^y") {
        resultSoFar = pow(resultSoFar, operand);
    } else {
        resultSoFar = operand;
    }
    
    display->setText(QString::number(resultSoFar));
}