#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

template <typename T = int64_t>
class GeneratorHacker {
public:
    GeneratorHacker(std::vector<T> & xVector)
        : xVector(xVector)
        , isGeneratorHacked(false) { }

    void hack() {
        int i = 0;
        while ((m = pow(2, i++)) < mMax) {
            try {
                xNext = solveForSpecificM(m);
                xVector.push_back(xNext);

                return;
            } catch (std::exception&) {
                continue;
            }
        }

        for (m = 2; m < mMax; ++m) {
            try {
                xNext = solveForSpecificM(m);
                xVector.push_back(xNext);

                return;
            } catch (std::exception&) {
                continue;
            }
        }

        /* if code is here
            it means no solutions*/

        //throw std::exception();
    }

    inline void printGenerator() {
        if (isGeneratorHacked) {
           std::cout << "Hacked generator: X[n+1] = X[n]*" \
                << k << " + " << a << " (mod " << m << ")\n";
        } else {
            std::cout << "Generator is not hacked\n";
        }
    }

private:
    std::vector<T> & xVector;

    T a, k, m;    // parameters to be hacked
    T xNext;      // new element to xVector must be counted

    const T mMax = 65536;

public:
    bool isGeneratorHacked;

private:
    T solveForSpecificM(T m) {
//        std::clog << "m = " << m << std::endl;

        T l = xVector[2] - xVector[1];
        T r = xVector[1] - xVector[0];
        T d = std::__gcd(r, m);

        if (l % d != 0) {
            throw std::exception();
        }

        while (l % r != 0) {
            l += m;
        }

        T k = T(l / r);
        T a = (xVector[2] - xVector[1] * k) % m;
        if (a < 0) {
            a += m;
        }

        bool check1 = (xVector[3] == ((xVector[2] * k + a) % m));
        bool check2 = (xVector[2] == ((xVector[1] * k + a) % m));
        bool check3 = (xVector[1] == ((xVector[0] * k + a) % m));

        if (check1 & check2 & check3) {
            this->k = k;
            this->a = a;
            this->m = m;
            this->isGeneratorHacked = true;

            return (xVector[3] * k + a) % m;
        }

        throw std::exception();
    }
};

int main()
{
    std::vector<int64_t> xVector;
    xVector.resize(4);

    std::cout << "Enter x sequence (4 elements through a space): ";
    for (int i = 0; i < xVector.size(); ++i) {
        std::cin >> xVector[i];
    }

    GeneratorHacker<int64_t> generatorHacker(xVector);
    generatorHacker.hack();
    generatorHacker.printGenerator();

    if (generatorHacker.isGeneratorHacked) {
        std::cout << "Next x: " << xVector.back() << std::endl;
    } else {
        std::cout << "Next x: undefined" << std::endl;
    }

    return 0;
}
