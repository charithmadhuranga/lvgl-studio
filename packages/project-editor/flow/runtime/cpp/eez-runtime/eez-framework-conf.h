#pragma once

#include <stdint.h>

extern uint32_t DISPLAY_WIDTH;
extern uint32_t DISPLAY_HEIGHT;
static const uint32_t DISPLAY_BPP = 32; // RGBA8888

static const char *TITLE = "Min EEZ Sample";
static const char *ICON = "min_eez_sample.png";

#define MAX_NUM_OF_Y_AXES 18
#define MAX_NUM_OF_Y_VALUES MAX_NUM_OF_Y_AXES

#define OPTION_KEYBOARD 0
#define OPTION_MOUSE 0

#define CUSTOM_VALUE_TYPES

#define DISPLAY_BACKGROUND_LUMINOSITY_STEP_MIN 0
#define DISPLAY_BACKGROUND_LUMINOSITY_STEP_MAX 20
#define DISPLAY_BACKGROUND_LUMINOSITY_STEP_DEFAULT 10

static const uint32_t GUI_STATE_BUFFER_SIZE = 128 * 1024;

#define OPTION_KEYPAD 1

#define EEZ_DASHBOARD_API

#define EEZ_STUDIO_FLOW_RUNTIME
