#include "core/Log.h"

int main()
{
    TE_LOG_INFO("TerranEngine bootstrap v{}", "0.1.1");
    TE_LOG_DEBUG("Screen size set to {}x{}", 480, 270);
    return 0;
}