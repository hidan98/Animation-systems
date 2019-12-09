#pragma once

enum MyNodeTypes {
	ikNode = 0,
	LinearBlendNode,
	BledNode2D,
	ragDoll,
	transNode,
	clipNode,
#   ifdef IMGUI_USE_AUTO_BINDING
	MNT_TEXTURE_NODE,
#   endif
	//outputNodeType,

	MNT_OUTPUT_NODE,    // One problem here when adding new values is backward compatibility with old saved files: they rely on the previously used int values (it should be OK only if we append new values at the end).
	MNT_COUNT

};