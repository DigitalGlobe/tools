/******************************************************************************
 *
 * Component: OGDI VRF Driver
 * Purpose: Implementation of vrf Server open, close and rewind functions
 * 
 ******************************************************************************
 * Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of L.A.S. Inc not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission. L.A.S. Inc. makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: open.c,v $
 * Revision 1.6  2001/06/21 20:30:15  warmerda
 * added ECS_CVSID
 *
 * Revision 1.5  2001/06/13 17:33:59  warmerda
 * upgraded source headers
 *
 */

#include "ecs.h"
#include "vrf.h"

ECS_CVSID("$Id: open.c,v 1.6 2001/06/21 20:30:15 warmerda Exp $");

/*
 *  --------------------------------------------------------------------------
 *  _openAreaLayer: 
 *   
 *      open an initialize a new vrf area vector layer
 *  --------------------------------------------------------------------------
 */

void
_openAreaLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}

void
_closeAreaLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}

void
_rewindAreaLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}

/*
 *  --------------------------------------------------------------------------
 *  _openLineLayer: 
 *   
 *      open an initialize a new vrf line vector layer
 *  --------------------------------------------------------------------------
 */

void
_openLineLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}

void
_closeLineLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}

void
_rewindLineLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}


/*
 *  --------------------------------------------------------------------------
 *  _openPointLayer: 
 *   
 *      open an initialize a new vrf sites layer
 *  --------------------------------------------------------------------------
 */

void
_openPointLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}


void
_closePointLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}

void
_rewindPointLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}


/*
 *  --------------------------------------------------------------------------
 *  _openTextLayer: 
 *   
 *      open an initialize a new vrf paint/label layer
 *  --------------------------------------------------------------------------
 */

void
_openTextLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}


void
_closeTextLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}


void
_rewindTextLayer(s,l)
	ecs_Server *s;
	ecs_Layer *l;
{
    (void) s;
    (void) l;
}






