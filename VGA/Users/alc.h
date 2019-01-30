#ifndef AL_ALC_H
#define AL_ALC_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <vgmdef.h>
#include <al.h>
	
typedef struct
{
	f32 speedOfSound;
	f32 dopplerVelocity;
	f32 dopplerFactor;
	void *headSource;
	void *tailSource;
	void *headBuffer;
	void *tailBuffer;	

	void *lastSource;
	void *lastBuffer;
	ALenum error;
}ALCcontext;

typedef struct
{
	u8 test;
}ALCdevice;

extern ALCdevice* alcOpenDevice(const char *devicename);
extern void alcCloseDevice(ALCdevice* device);
extern ALCcontext* alcCreateContext(ALCdevice* device, const char *attr);
extern ALvoid alcDestroyContext(ALCcontext *context);
extern void alcMakeContextCurrent(ALCcontext*);
extern ALCcontext* alcGetContextCurrent(void);

#if defined(__cplusplus)
};
#endif

#endif
