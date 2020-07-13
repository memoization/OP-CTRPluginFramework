#ifndef CTRPLUGINFRAMEWORK_SYSTEM_SERVICES_ENV_HPP
#define CTRPLUGINFRAMEWORK_SYSTEM_SERVICES_ENV_HPP

#include "types.h"
#include <string>

namespace CTRPluginFramework
{
    namespace Services
    {
        /**
         * @brief Add a service handle to the env list shared with ctrulib
         * @param srv The name of the service
         * @param handle The handle of the service
         */
        void    AddServiceToEnv(const std::string& srv, Handle handle);
    }
}

#endif
