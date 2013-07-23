/***************************************************************************\
 *
 *               (C) copyright Fraunhofer - IIS (2012)
 *                        All Rights Reserved
 *
 *   Project:  CoreAudio AAC-ELD Example Source Code
 *
 *   By using this Example Source Code, you agree to the "Terms & Conditions 
 *   for Use of Fraunhofer Example Source Code", which is provided as a 
 *   separate document together with this file. The "Terms & Conditions for 
 *   Use of Fraunhofer Example Source Code" must be part of any redistribution
 *   of the Example Source Code or parts thereof, or modifications of the 
 *   Example Source Code.
 *
 \***************************************************************************/

#ifndef AUDIO_UNIT_IO_H
#define AUDIO_UNIT_IO_H

#include <AudioToolbox/AudioToolbox.h>

/* Initialize the AudioUnit API */
void InitAudioUnit();
/* Start AudioUnit processing */
void StartAudioUnit();
/* Stop AudioUnit processing */
void StopAudioUnit();

#endif /* AUDIO_UNIT_IO_H */