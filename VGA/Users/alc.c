#include <alc.h>
#include <stdlib.h>
#include <vgmdef.h>
#ifdef STM32
#include <timer.h>
#else
#endif

static ALCcontext *currentContext = 0;

ALCdevice* alcOpenDevice(const char *devicename)
{
	ALCdevice* aLCdevice = (ALCdevice* )malloc(sizeof(ALCdevice));
	memset(aLCdevice, 0, sizeof(ALCdevice));
	
	return aLCdevice;
}

void alcCloseDevice(ALCdevice* device)
{
	if(device)
	{
		free(device);
		device = 0;
	}
}

#ifdef STM32	
u32 counter = 0;
//u32 sampleRate = 44100;
u32 sampleRate = 22050;
//u32 sampleRate = 11025;
u32 bpp = 10;
//u32 bpp = 11;
//u32 bpp = 12;
u32 maxSteps = 1024;//512//pow(2, bpp);
//u32 maxSteps = 2048;//1024//pow(2, bpp);
//u32 maxSteps = 4096;//2048//pow(2, bpp);	
u32 interruptsPerUpdate = 3.18877551; // 1.594387755 // 72*1000*1000 / max / sampleRate;
#endif

extern alSourceUpdate(void *head);
void alContextUpdate(void)
{
	if(counter++>interruptsPerUpdate)
	{
		counter = 0;
		
		if(currentContext)
			alSourceUpdate(currentContext->headSource);
	}
}

ALCcontext* alcCreateContext(ALCdevice* device, const char *attr)
{
	ALCcontext* context = (ALCcontext* )malloc(sizeof(ALCcontext));
	memset(context, 0, sizeof(ALCcontext));
	if(!context)
		return 0;
	
	context->speedOfSound = 330;
	context->dopplerVelocity = 0;
	context->dopplerFactor = 1;
	context->headSource = 0;
	context->tailSource = 0;
	context->headBuffer = 0;
	context->tailBuffer = 0;
	context->lastSource = 0;
	context->lastBuffer = 0;
	context->error 		= AL_NO_ERROR;

#ifdef STM32
	TIMER_Initialize(maxSteps, 0, alContextUpdate); 	// Frequency is 720kHz: Tout = (arr+1)(psc+1)/TClk, assume APB1 no prescale, TClk 72Mhz
								// 72000000/(99+1) = 720KHZ, no prescale clock
	//GPIOB->BRR = GPIO_Pin_1;

	//TIMER_PWM_Initialize(1);
	//TIMER_PWM_Initialize(2);
	TIMER_PWM_Initialize(3);
	TIMER_PWM_Initialize(4);
	//DMA_Initialize();
#else
#endif
	return context;
}

ALvoid alcDestroyContext(ALCcontext *context)
{
	if(context)
	{
		// TIMER_PWM_ShutDown();
		// TIMER_ShutDown();
		free(context);
		context = 0;
	}
}

void alcMakeContextCurrent(ALCcontext* context)
{
	if(context)
	{
		currentContext = context;
	}
}

ALCcontext* alcGetContextCurrent(void)
{
	return currentContext;
}
