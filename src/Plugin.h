#ifndef __PLUGIN_H__
#define __PLUGIN_H__ 1

#include "PluginLibrary.h"

class PluginLibrary;

class Plugin
{
    private:
        int _id;
        const PluginLibrary& _library;

    public:
        Plugin(const PluginLibrary& library, int id);
        void start();
        void argument(std::string key, std::string value);

        virtual ~Plugin();
};

#endif
