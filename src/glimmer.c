#include "glimmer/glimmer.h"

#include <string.h>

int glimmerInitContext(GlimmerContext* ctx)
{
    memset(ctx, 0, sizeof(GlimmerContext));

    return 0;
}
