#ifndef CTRPLUGINFRAMEWORK_RENDER_HPP
#define CTRPLUGINFRAMEWORK_RENDER_HPP

#include "string"

namespace CTRPluginFramework
{
    class Render
    {
    private:

    public:
        /**
        * \brief Gets the amount of pixels the text takes using the sysfont.
        * \param text Input text to calculate its width
        * \return Value of pixels the text takes.
        */
        static float GetTextWidth(const std::string& text);
    };

}

#endif