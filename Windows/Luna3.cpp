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
#include <string.h>

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
	static UINT UpperLensCoverGrp[1] = {0};

	static MGROUP_ROTATE UpperLensCover (
		0, 
		UpperLensCoverGrp,
		1,
		_V(-0.0010, 0.1447, -0.5240), 
		_V(1,0,0), (float)(3.31613));
		
	static UINT LowerLensCoverGrp[1] = {1};

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
	THRUSTER_HANDLE th_rcs[24], th_group[40];

	//Physical vessel parameters
	SetSize (Luna3_SIZE);
	SetEmptyMass (Luna3_EMPTY_MASS);
	AddMesh("Luna3");

	//Propellant resources
	PROPELLANT_HANDLE hpr = CreatePropellantResource(Luna3_FUELMASS);


	// RCS engines
	th_rcs[ 0] = CreateThruster (_V( 1,0, 3), _V(0, 1,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[ 1] = CreateThruster (_V( 1,0, 3), _V(0,-1,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[ 2] = CreateThruster (_V(-1,0, 3), _V(0, 1,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[ 3] = CreateThruster (_V(-1,0, 3), _V(0,-1,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[ 4] = CreateThruster (_V( 1,0,-3), _V(0, 1,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[ 5] = CreateThruster (_V( 1,0,-3), _V(0,-1,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[ 6] = CreateThruster (_V(-1,0,-3), _V(0, 1,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[ 7] = CreateThruster (_V(-1,0,-3), _V(0,-1,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[ 8] = CreateThruster (_V( 1,0, 3), _V(-1,0,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[ 9] = CreateThruster (_V(-1,0, 3), _V( 1,0,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[10] = CreateThruster (_V( 1,0,-3), _V(-1,0,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[11] = CreateThruster (_V(-1,0,-3), _V( 1,0,0), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[12] = CreateThruster (_V( 0,0,-3), _V(0,0, 1), Luna3_MAXRCSTH, hpr, Luna3_ISP);
	th_rcs[13] = CreateThruster (_V( 0,0, 3), _V(0,0,-1), Luna3_MAXRCSTH, hpr, Luna3_ISP);

	th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[2];
	th_group[2] = th_rcs[5];
	th_group[3] = th_rcs[7];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_PITCHUP);

	th_group[0] = th_rcs[1];
	th_group[1] = th_rcs[3];
	th_group[2] = th_rcs[4];
	th_group[3] = th_rcs[6];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_PITCHDOWN);

	th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[4];
	th_group[2] = th_rcs[3];
	th_group[3] = th_rcs[7];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_BANKLEFT);

	th_group[0] = th_rcs[1];
	th_group[1] = th_rcs[5];
	th_group[2] = th_rcs[2];
	th_group[3] = th_rcs[6];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_BANKRIGHT);

	th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[4];
	th_group[2] = th_rcs[2];
	th_group[3] = th_rcs[6];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_UP);

	th_group[0] = th_rcs[1];
	th_group[1] = th_rcs[5];
	th_group[2] = th_rcs[3];
	th_group[3] = th_rcs[7];
	CreateThrusterGroup (th_group, 4, THGROUP_ATT_DOWN);

	th_group[0] = th_rcs[8];
	th_group[1] = th_rcs[11];
	CreateThrusterGroup (th_group, 2, THGROUP_ATT_YAWLEFT);

	th_group[0] = th_rcs[9];
	th_group[1] = th_rcs[10];
	CreateThrusterGroup (th_group, 2, THGROUP_ATT_YAWRIGHT);

	th_group[0] = th_rcs[8];
	th_group[1] = th_rcs[10];
	CreateThrusterGroup (th_group, 2, THGROUP_ATT_LEFT);

	th_group[0] = th_rcs[9];
	th_group[1] = th_rcs[11];
	CreateThrusterGroup (th_group, 2, THGROUP_ATT_RIGHT);

	CreateThrusterGroup (th_rcs+12, 1, THGROUP_ATT_FORWARD);
	CreateThrusterGroup (th_rcs+13, 1, THGROUP_ATT_BACK);
}

// --------------------------------------------------------------
// Read status from scenario file
// --------------------------------------------------------------
void Luna3::clbkLoadStateEx (FILEHANDLE scn, void *vs)
{
	char *line;

	while (oapiReadScenario_nextline (scn, line)) {
		if (!_strnicmp (line, "ANT", 3)) {
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
			if (lens_cover_proc > 0.0) lens_cover_proc = max (0.0, lens_cover_proc-da);
			else                lens_cover_status = DOOR_CLOSED;
		} else {
			if (lens_cover_proc < 1.0) lens_cover_proc = min (1.0, lens_cover_proc+da);
			else                lens_cover_status = DOOR_OPEN;
		}
		SetAnimation (anim_lens_cover, lens_cover_proc);
	}

}

// --------------------------------------------------------------
// Keyboard interface handler (buffered key events)
// --------------------------------------------------------------
int Luna3::clbkConsumeBufferedKey (DWORD key, bool down, char *kstate)
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
