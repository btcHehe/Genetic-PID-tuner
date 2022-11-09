#include "utils.hpp"

double abs(double x) {
    if (x < 0.0) {
        return -1.0 * x;
    } else {
        return x;
    }
}
