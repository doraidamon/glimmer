#ifndef _GLIMMER_GLIMMER_H_
#define _GLIMMER_GLIMMER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int (*init_driver)();
} GlimmerDriverVtable;

typedef struct {
    GlimmerDriverVtable vtable;
} GlimmerDriver;

typedef struct {
    int initialized;
    GlimmerDriver* driver;
} GlimmerContext;

int glimmerInitContext(GlimmerContext* ctx);
int glimmerSetDriver(GlimmerContext* ctx, GlimmerDriver* driver);

typedef struct {

} GlimmerSurface;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* #ifndef _GLIMMER_GLIMMER_H_ */
