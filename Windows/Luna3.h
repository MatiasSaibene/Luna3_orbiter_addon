// Copyright (c) Matías Saibene
// Licensed under the MIT License

// ==============================================================
//                 ORBITER MODULE: Luna3
//
// Luna3.cpp
// Let's cross our fingers and pray this works...
//
// Luna3 mesh and textures by Matías Saibene
// ==============================================================

#ifndef __LUNA3_H
#define __LUNA3_H

#define STRICT
#include "Orbitersdk.h"

// ==============================================================
// Some parameters
// ==============================================================

const double Luna3_SIZE = 1.3;
const double Luna3_EMPTY_MASS = 278.5;
const double Luna3_FUELMASS = 100.0;
const double Luna3_ISP = 1000;
const double Luna3_MAXRCSTH = 100;
const double LENS_COVER_OPERATING_SPEED = 0.025;

// ==============================================================
// Luna3 class interface
// ==============================================================

class Luna3: public VESSEL3 {
public:
	enum DoorStatus { DOOR_CLOSED, DOOR_OPEN, DOOR_CLOSING, DOOR_OPENING } lens_cover_status;
	Luna3 (OBJHANDLE hObj, int fmodel);
	void DefineAnimations (void);
	void ActivateLensCover (DoorStatus action);
	void RevertLensCover (void);

	// Overloaded callback functions
	virtual void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkLoadStateEx (FILEHANDLE scn, void *vs);
	virtual void clbkSaveState (FILEHANDLE scn) override;
	virtual void clbkPostStep (double simt, double simdt, double mjd) override;
	virtual int clbkConsumeBufferedKey (DWORD key, bool down, char *kstate) override;

private:
	UINT anim_lens_cover;
	double lens_cover_proc;
};

#endif // !__LUNA3_H