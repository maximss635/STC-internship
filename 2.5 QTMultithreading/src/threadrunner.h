#ifndef THREADRUNNER_H
#define THREADRUNNER_H

#include <QThread>
#include <QDebug>

class SolverThread : public QThread {
    Q_OBJECT
public:
    SolverThread();
    ~SolverThread();

    // thread-entry code
    void run() override;

    inline void setNumber(uint64_t number) {
        this->checkedNumber = number;
    }

    void setMaxItterations(uint64_t itterations);

private:
    uint64_t powMod(
            uint64_t base,
            uint64_t exponent,
            uint64_t module);

private:
    uint64_t checkedNumber;
    uint maxItterations;

    static int threadsTotal;
    const int threadId;

public:
signals:
    void endItteration(int threadId, int percentProgress);
    void endWork(int threadId, bool isNumberPrime, uint64_t base = -1);
};


class ThreadRunner
{
public:
    ThreadRunner(uint64_t checkedNumber, uint maxItterationsInThread);

    void runEvery();
    void waitEvery();

    inline SolverThread &getThread(int index) {
        return threadPool[index];
    }

private:
    static const int threadNumber = 10;
    SolverThread threadPool[threadNumber];

    uint64_t checkedNumber;
};


#endif // THREADRUNNER_H
