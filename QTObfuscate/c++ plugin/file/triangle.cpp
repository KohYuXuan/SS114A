#include "polygon.hpp"
#include <cmath>


class triangle : public polygon {
public:
    virtual double area()  {
side_length_ = 100.00;

        return side_length_ * side_length_ * sqrt(3) / 2;
    }

    virtual double get_side_length() const {

        return side_length_;
}
};


// the class factories

extern "C" polygon* create() {
    return new triangle;
}

extern "C" void destroy(polygon* p) {
    delete p;
}
