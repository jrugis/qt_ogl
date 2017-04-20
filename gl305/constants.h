#ifndef CONSTANTS_H
#define CONSTANTS_H

#define ANIMATE_REFRESH_RATE 29.0
#define ANIMATE_PERIOD (1000.0 / ANIMATE_REFRESH_RATE)
#define ANIMATE_MS_DEGREE 50.0

#define MAX_DATA_VERTS 100
#define PLOT_RANGE 10
#define PLOT_ZERO 5.0, 5.0

#define MAX_SOURCE_VALS 100

#define CAMERA2VIEW_PERSPECTIVE 20.0f, wh, 0.0f, 10.0f
#define MODEL2WORLD_SCALE 1.5f
#define MODEL2WORLD_TRANSLATE -0.5f, -0.5f, 0.0f

#define BLACK3 0.0f, 0.0f, 0.0f
#define WHITE3 1.0f, 1.0f, 1.0f

#define VERY_LIGHT_GRAY 0.9f, 0.9f, 0.9f
#define LIGHT_GRAY 0.8f, 0.8f, 0.8f
#define MED_GRAY 0.6f, 0.6f, 0.6f
#define DARK_GRAY 0.4f, 0.4f, 0.4f

#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define YELLOW 1.0f, 1.0f, 0.0f
#define CYAN 0.0f, 1.0f, 1.0f
#define MAGENTA 1.0f, 0.0f, 1.0f

#endif // CONSTANTS_H
