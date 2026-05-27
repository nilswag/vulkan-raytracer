#pragma once
#include "vulkan/app.h"

typedef struct
{
	App vulkan_app;

	size_t ticks;
	float dt;
} State;

extern State state;