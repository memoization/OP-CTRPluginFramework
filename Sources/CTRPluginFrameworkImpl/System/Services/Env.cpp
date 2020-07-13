#include <3ds.h>
#include "types.h"
#include <string>
#include <map>

namespace CTRPluginFramework
{
    namespace Services
    {
        static std::map<const std::string, Handle>     g_serviceList;

        void    AddServiceToEnv(const std::string& srv, Handle handle)
        {
            g_serviceList.insert(std::make_pair(srv, handle));
        }
    }
}

// ctrulib override to share handles
extern "C"
{
    using namespace CTRPluginFramework::Services;

    Handle  envGetHandle(const char* name)
    {
        auto it = g_serviceList.find(name);

        if (it == g_serviceList.end())
            return 0;

        return it->second;
    }

    void    envDestroyHandles(void)
    {
        for (const auto& it : g_serviceList)
        {
            svcCloseHandle(it.second);
        }

        g_serviceList.clear();
    }
}
