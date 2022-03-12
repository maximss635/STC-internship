#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QLabel>
#include <QProgressBar>
#include <QMutex>

#include "threadrunner.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void buttonRunHandler();
    void lineEditHandler();

    void callBackThreadIter(int threadId, int progress) {
        threadProgress[threadId] = progress;
    }

    // !!!! this function can be called simultaneously in different threads !!!!
    void callBackThreadOut(int threadId, bool isNumberPrime, uint64_t base = -1);

private:
    void clearWidgets();

private:
    Ui::MainWindow *ui;
    ThreadRunner* threadRunner;

    uint maxItterationsInThread;

private:
    QLabel* threadLabels[10];
    QProgressBar* threadProgressBars[10];
    QMutex mutex;

    bool isCheckedNumberPrime;
    int threadProgress[10];

private:
    int readyThreads;
};
#endif // MAINWINDOW_H
