// Copyright (c) Matías Saibene
// Licensed under the MIT License

// ==============================================================
//                    ORBITER MODULE: LUNA3
//
// Luna3.cpp
// Let's cross our fingers and pray this works...
//
// Luna3 mesh and textures by Matías Saibene
// ==============================================================

#define ORBITER_MODULE

#include "Luna3.h"
#include <stdio.h>
#include <strings.h>

// --------------------------------------------------------------
// Constructor
// --------------------------------------------------------------
Luna3::Luna3 (OBJHANDLE hObj, int fmodel)
: VESSEL3 (hObj, fmodel)
{
	lens_cover_proc = 0.0;
	lens_cover_status = DOOR_CLOSED;
	DefineAnimations ();
}

// --------------------------------------------------------------
// Define animation sequences for moving parts
// --------------------------------------------------------------
void Luna3::DefineAnimations (void)
{
	// 1. Lens cover opening animation
	static unsigned int UpperLensCoverGrp[1] = {0};

	static MGROUP_ROTATE UpperLensCover (
		0, 
		UpperLensCoverGrp,
		1,
		_V(-0.0010, 0.1447, -0.5240), 
		_V(1,0,0), (float)(3.31613));
		
	static unsigned int LowerLensCoverGrp[1] = {1};

	static MGROUP_ROTATE LowerLensCover 
	(0,
	LowerLensCoverGrp,
	1,
	_V(-0.0010, -0.1365, -0.5240),
	_V(-1,0,0), (float)(3.31613));

	anim_lens_cover = CreateAnimation (0.0196);

	AddAnimationComponent (anim_lens_cover, 0, 0.5, &UpperLensCover);
	AddAnimationComponent (anim_lens_cover, 0, 1,   &LowerLensCover);
}

void Luna3::ActivateLensCover (DoorStatus action)
{
	lens_cover_status = action;
}

void Luna3::RevertLensCover (void)
{
	ActivateLensCover ((lens_cover_status == DOOR_CLOSED || lens_cover_status == DOOR_CLOSING) ?
		DOOR_OPENING : DOOR_CLOSING);
}

// ==============================================================
// Overloaded callback functions
// ==============================================================

// --------------------------------------------------------------
// Set vessel class parameters
// --------------------------------------------------------------
void Luna3::clbkSetClassCaps (FILEHANDLE cfg)
{
	SetSize (Luna3_SIZE);
	SetEmptyMass (Luna3_EMPTY_MASS);
	AddMesh("Luna3");
}

// --------------------------------------------------------------
// Read status from scenario file
// --------------------------------------------------------------
void Luna3::clbkLoadStateEx (FILEHANDLE scn, void *vs)
{
	char *line;

	while (oapiReadScenario_nextline (scn, line)) {
		if (!strncasecmp (line, "ANT", 3)) {
			sscanf (line+3, "%d%lf", (int *)&lens_cover_status, &lens_cover_proc);
		} else {
			ParseScenarioLineEx (line, vs);
		}
	}

	SetAnimation (anim_lens_cover, lens_cover_proc);
}

// --------------------------------------------------------------
// Save status to scenario file
// --------------------------------------------------------------
void Luna3::clbkSaveState (FILEHANDLE scn)
{
	char cbuf[256];
	SaveDefaultState (scn);
	sprintf (cbuf, "%d %0.4f", lens_cover_status, lens_cover_proc);
	oapiWriteScenario_string (scn, "ANT", cbuf);
}

// --------------------------------------------------------------
// Frame update
// --------------------------------------------------------------
void Luna3::clbkPostStep (double simt, double simdt, double mjd)
{
	// Animate hi-gain antenna
	if (lens_cover_status >= DOOR_CLOSING) {
		double da = simdt * LENS_COVER_OPERATING_SPEED;
		if (lens_cover_status == DOOR_CLOSING) {
			if (lens_cover_proc > 0.0) lens_cover_proc = std::max (0.0, lens_cover_proc-da);
			else                lens_cover_status = DOOR_CLOSED;
		} else {
			if (lens_cover_proc < 1.0) lens_cover_proc = std::min (1.0, lens_cover_proc+da);
			else                lens_cover_status = DOOR_OPEN;
		}
		SetAnimation (anim_lens_cover, lens_cover_proc);
	}
}

// --------------------------------------------------------------
// Keyboard interface handler (buffered key events)
// --------------------------------------------------------------
int Luna3::clbkConsumeBufferedKey (int key, bool down, char *kstate)
{
	if (!down) return 0; // only process keydown events

	if (KEYMOD_CONTROL (kstate)) {

		switch (key) {
		case OAPI_KEY_1: // deploy/retract antenna
			RevertLensCover();
			return 1;
		}
	}
	return 0;
}

// ==============================================================
// API callback interface
// ==============================================================

// --------------------------------------------------------------
// Vessel initialisation
// --------------------------------------------------------------
DLLCLBK VESSEL *ovcInit (OBJHANDLE hvessel, int flightmodel)
{
	return new Luna3 (hvessel, flightmodel);
}

// --------------------------------------------------------------
// Vessel cleanup
// --------------------------------------------------------------
DLLCLBK void ovcExit (VESSEL *vessel)
{
	if (vessel) delete (Luna3*)vessel;
}
