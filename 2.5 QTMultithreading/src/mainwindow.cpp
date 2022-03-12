#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , threadRunner(nullptr)
    , maxItterationsInThread(10000)
    , isCheckedNumberPrime(true)
    , readyThreads(0)
{
    ui->setupUi(this);
    // arrays with pointers to widgets to easy work (itterating throw)
    this->threadLabels[0] = ui->labelThread_1;
    this->threadLabels[1] = ui->labelThread_2;
    this->threadLabels[2] = ui->labelThread_3;
    this->threadLabels[3] = ui->labelThread_4;
    this->threadLabels[4] = ui->labelThread_5;
    this->threadLabels[5] = ui->labelThread_6;
    this->threadLabels[6] = ui->labelThread_7;
    this->threadLabels[7] = ui->labelThread_8;
    this->threadLabels[8] = ui->labelThread_9;
    this->threadLabels[9] = ui->labelThread_10;

    this->threadProgressBars[0] = ui->progressBarThread_1;
    this->threadProgressBars[1] = ui->progressBarThread_2;
    this->threadProgressBars[2] = ui->progressBarThread_3;
    this->threadProgressBars[3] = ui->progressBarThread_4;
    this->threadProgressBars[4] = ui->progressBarThread_5;
    this->threadProgressBars[5] = ui->progressBarThread_6;
    this->threadProgressBars[6] = ui->progressBarThread_7;
    this->threadProgressBars[7] = ui->progressBarThread_8;
    this->threadProgressBars[8] = ui->progressBarThread_9;
    this->threadProgressBars[9] = ui->progressBarThread_10;

    for (QProgressBar* progressBar : threadProgressBars) {
        progressBar->setValue(0);
    }

    connect(ui->buttonRun, &QPushButton::clicked, this, &MainWindow::buttonRunHandler);
    connect(ui->lineEditInputNumber, &QLineEdit::textChanged, this, &MainWindow::lineEditHandler);
    connect(ui->lineEditInputMaxIter, &QLineEdit::textChanged, this, &MainWindow::lineEditHandler);

    lineEditHandler();
    ui->lineEditInputNumber->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::buttonRunHandler()
{
    clearWidgets();
    bool success = true;

    uint64_t checkedNumber = ui->lineEditInputNumber->text()\
            .toULongLong(&success);
    if (!success || 0 == checkedNumber) {
        ui->lineEditInputNumber->setText("");
        return;
    }

    this->maxItterationsInThread = ui->lineEditInputMaxIter->text()\
            .toUInt(&success);
    if (!success || 0 == maxItterationsInThread) {
        ui->lineEditInputNumber->setText("");
        return;
    }

    // create pool with threads
    readyThreads = 0;
    ui->buttonRun->setEnabled(false);
    repaint();
    threadRunner = new ThreadRunner(checkedNumber, maxItterationsInThread);

    for (int i = 0; i < 10; ++i) {
        connect(
            &threadRunner->getThread(i),
            &SolverThread::endItteration,
            this,
            &MainWindow::callBackThreadIter,
            Qt::DirectConnection);

        connect(
            &threadRunner->getThread(i),
            &SolverThread::endWork,
            this,
            &MainWindow::callBackThreadOut,
            Qt::DirectConnection);
    }

    threadRunner->runEvery();
//    threadRunner->waitEvery();

    while (10 != readyThreads) {
        for (int i = 0; i < 10; ++i) {
            threadProgressBars[i]->setValue(threadProgress[i]);
        }

        if (!isCheckedNumberPrime && ui->labelResult->text().isEmpty()) {
            ui->labelResult->setText("Number is composite");
            //break;
        }
    }

    delete threadRunner;


    // all threads dies
    for (QProgressBar* progressBar : this->threadProgressBars) {
        progressBar->setValue(100);
    }

    if (ui->labelResult->text().isEmpty()) {
        ui->labelResult->setText("Numper is prime");
    }


    ui->buttonRun->setEnabled(true);
    repaint();
}

void MainWindow::lineEditHandler()
{
    ui->buttonRun->setEnabled(
        !ui->lineEditInputNumber->text().isEmpty() &&
        !ui->lineEditInputMaxIter->text().isEmpty()
    );

    repaint();
}

void MainWindow::callBackThreadOut(int threadId, bool isNumberPrime, uint64_t base)
{
       qDebug() << "callBackThreadOut " << threadId << isNumberPrime;

       mutex.lock();
       ++readyThreads;

       threadLabels[threadId]->setText(
           (isNumberPrime ?
           "Number is prime" :
           ("Number is composite, base = " + QString::number(static_cast<qulonglong>(base))))
       );
       mutex.unlock();

       if (!isNumberPrime) {
           this->isCheckedNumberPrime = false;
       }
   }


void MainWindow::clearWidgets()
{
    // clear progress bars
    for (QProgressBar* progressBar : this->threadProgressBars) {
        progressBar->setValue(0);
    }

    // clear labels
    for (QLabel* label : this->threadLabels) {
        label->setText("");
    }
    ui->labelResult->setText("");
    repaint();

    this->isCheckedNumberPrime = true;
}
