#include "PluginManager.h"

void PluginManager::loadPlugin(const std::string& pluginName)
{
    // load the triangle library
    void* triangle = dlopen(pluginName.c_str(), RTLD_LAZY);
    if (!triangle) {
        cerr << "Cannot load library: " << dlerror() << '\n';
        //return 1;
    }

    // load the symbols
    create_t* create_triangle = (create_t*) dlsym(triangle, "create");
    destroy_t* destroy_triangle = (destroy_t*) dlsym(triangle, "destroy");
    if (!create_triangle || !destroy_triangle) {
        cerr << "Cannot load symbols: " << dlerror() << '\n';
        //return 1;
    }

    // create an instance of the class
    Plugin* poly = create_triangle();
    cout << "before poly->algorithm(); " << endl;
    // use the class
    poly->setEngine(g_engine);
    poly->setUserOptions(userOptions);

    poly->algorithm();

    cout << "after  poly->algorithm(); " << endl;
    // destroy the class
    destroy_triangle(poly);

    // unload the triangle library
    dlclose(triangle);
}
