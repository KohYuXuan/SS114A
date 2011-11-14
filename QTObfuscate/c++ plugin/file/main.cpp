#include "polygon.hpp"
#include <iostream>
#include <dlfcn.h>
#include <string>
//#include "File.h"
using namespace std;


//void print();

int main() {
/*
print();
string abc;
File file;
file.loadFile("hello.txt", abc);
cout << abc << endl;
*/
    using std::cout;
    using std::cerr;


    // load the triangle library
    void* triangle = dlopen("./triangle.so", RTLD_LAZY);
    if (!triangle) {
        cerr << "Cannot load library: " << dlerror() << '\n';
        return 1;
    }

    // load the symbols
    create_t* create_triangle = (create_t*) dlsym(triangle, "create");
    destroy_t* destroy_triangle = (destroy_t*) dlsym(triangle, "destroy");
    if (!create_triangle || !destroy_triangle) {
        cerr << "Cannot load symbols: " << dlerror() << '\n';
        return 1;
    }

    // create an instance of the class
    polygon* poly = create_triangle();

    // use the class
    poly->set_side_length(7);
poly->area();
        cout << "The area is: " <<poly->get_side_length() << '\n';

    // destroy the class
    destroy_triangle(poly);

    // unload the triangle library
    dlclose(triangle);

}

