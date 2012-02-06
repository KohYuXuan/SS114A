#include "PluginManager.h"

int PluginManager::loadPlugin(const std::string& pluginName)
{
    void* algorithm = dlopen(pluginName.c_str(), RTLD_LAZY);
    if (!algorithm) {
        cerr << "Cannot load library: " << dlerror() << '\n';
        return CANNOT_LOAD_PLUGIN;
    }

    // load the symbols
    create_t* create_algorithm = (create_t*) dlsym(algorithm, "create");
    destroy_t* destroy_algorithm = (destroy_t*) dlsym(algorithm, "destroy");
    if (!create_algorithm || !destroy_algorithm) {
        cerr << "Cannot load symbols: " << dlerror() << '\n';
        return CANNOT_LOAD_PLUGIN;
    }

    // create an instance of the class
    Plugin* plugin = create_algorithm();

    // use the class
    plugin->setEngine(g_engine);
    plugin->setUserOptions(userOptions);

    if(!plugin->algorithm()) {
            destroy_algorithm(plugin);
            dlclose(algorithm);
           return ALGORITHM_FAILED;
    }

    // destroy the class
    destroy_algorithm(plugin);

    // unload the algorithm library
    dlclose(algorithm);

    return 0;
}
