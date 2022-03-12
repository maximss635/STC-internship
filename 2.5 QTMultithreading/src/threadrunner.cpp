#include <QDebug>
#include <QApplication>
#include <QTime>

#include "threadrunner.h"

int SolverThread::threadsTotal = 0;

ThreadRunner::ThreadRunner(uint64_t checkedNumber, uint maxItterationsInThread)
    : checkedNumber(checkedNumber)
{
    for (SolverThread& t : threadPool) {
        t.setNumber(checkedNumber);
        t.setMaxItterations(maxItterationsInThread);
    }
}

void ThreadRunner::runEvery() {
    for (SolverThread& t : threadPool) {
        t.start();
    }
}

void ThreadRunner::waitEvery() {
    for (SolverThread& t : threadPool) {
        t.wait();
    }
}


SolverThread::SolverThread()
    : checkedNumber(0)
    , threadId(threadsTotal++) { }

SolverThread::~SolverThread() {
    --threadsTotal;
}

void SolverThread::setMaxItterations(uint64_t itterations) {
    this->maxItterations = itterations;
}

void SolverThread::run() {
    // thread-entry code
    qDebug() << "Thread start: " << threadId;
    qsrand(threadId * static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));

    uint64_t base, r;

    for (uint i = 0; i < SolverThread::maxItterations; ++i) {
        //qDebug() << "[thread" << threadId << "] [iter" << i << "]";

        base = qrand() % (checkedNumber - 1);
        if (base % checkedNumber == 0) {
            continue;
        }

        r = powMod(base, checkedNumber-1, checkedNumber);

        if (1 != r) {
            emit endItteration(threadId, 100);
            emit endWork(threadId, false, base);
            return;
        }

        emit endItteration(threadId, 100 * (i+1) / maxItterations);
    }

    emit endWork(threadId, true);
}


uint64_t SolverThread::powMod(
        uint64_t base,
        uint64_t exponent,
        uint64_t module) {

    if (module == 1) {
        return 0;
    }

    uint64_t result = 1;
    base %= module;
    for (; exponent > 0; exponent >>= 1) {
        if ((exponent & 1) == 1) {
            result = (result * base) % module;
        }
        base = (base * base) % module;
    }

    return result;
}
