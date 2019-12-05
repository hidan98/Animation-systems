#pragma once

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <platform/d3d11/system/platform_d3d11.h>
#include <platform\d3d11\input\keyboard_d3d11.h>




enum MyNodeTypes {
	MNT_COLOR_NODE = 0,
	MNT_COMBINE_NODE,
	MNT_COMMENT_NODE,
	MNT_COMPLEX_NODE,
#   ifdef IMGUI_USE_AUTO_BINDING
	MNT_TEXTURE_NODE,
#   endif
	MNT_OUTPUT_NODE,    // One problem here when adding new values is backward compatibility with old saved files: they rely on the previously used int values (it should be OK only if we append new values at the end).
	MNT_COUNT
};


class nodeGraph
{
public:
	nodeGraph();
	~nodeGraph();

	
	void update();

private:
	
};

