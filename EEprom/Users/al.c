#include <al.h>
#include <alc.h>
#include <stdlib.h>
#include <vgmdef.h>
#ifdef STM32
#include <timer.h>
#else
#endif

#define SOURCE_QUEUE_MAX 8

struct ALBuffer;
struct ALBuffer
{
	u32 id;
	u32 freq;
	ALenum format;
	u32 ridx;
	u8* buf;
	u32 bufSize;
	
	struct ALBuffer *prev;
	struct ALBuffer *next;
};
static u32 alBufferGUID = 0;

struct ALSource;
struct ALSource
{
	u32 id;
	
	boolean looping;
	ALfloat gain;
	ALfloat pitch;
	ALenum sourceState;
	struct ALBuffer* queue[SOURCE_QUEUE_MAX];
	u32 queueHead;
	u32 queueTail;
	u32 processed;
	
	struct ALSource *prev;
	struct ALSource *next;
};
static u32 alSourceGUID = 0;

AL_API ALvoid AL_APIENTRY alSpeedOfSound(ALfloat value)
{
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);

	ctx->speedOfSound = value;
}

AL_API ALvoid AL_APIENTRY alDopplerVelocity(ALfloat value)
{
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);

	ctx->dopplerVelocity = value;
}

AL_API ALvoid AL_APIENTRY alDopplerFactor(ALfloat value)
{
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	ctx->dopplerFactor = value;
}

AL_API struct ALSource* AL_APIENTRY alFindSource(ALuint sourceId)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	if(sourceId==0)
		return 0;
	
	if(ctx->lastSource && ((struct ALSource*)ctx->lastSource)->id == sourceId)
	{
		return ((struct ALSource*)ctx->lastSource);
	}	
	
	source = (struct ALSource*)ctx->headSource;
	while(source)
	{
		if(source->id==sourceId)
		{
			ctx->lastSource = source;
			return source;
		}
		
		source = source->next;
	}
	
	return 0;
}

AL_API struct ALBuffer* AL_APIENTRY alFindBuffer(ALuint bufferId)
{
	struct ALBuffer* buffer;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	if(bufferId==0)
		return 0;
	
	if(ctx->lastBuffer && ((struct ALBuffer*)ctx->lastBuffer)->id == bufferId)
	{
		return ((struct ALBuffer*)ctx->lastBuffer);
	}
	
	buffer = (struct ALBuffer*)ctx->headBuffer;
	while(buffer)
	{
		if(buffer->id==bufferId)
		{
			ctx->lastBuffer = buffer;
			return buffer;
		}
		
		buffer = buffer->next;
	}
	
	return 0;
}

AL_API ALvoid AL_APIENTRY alSourcef(ALuint sourceId, ALenum param, ALfloat value)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		case AL_PITCH:
			source->pitch = value;
		break;
		case AL_GAIN:
			source->gain = value;
		break;
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alSource3f(ALuint sourceId, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alSourcefv(ALuint sourceId, ALenum param, const ALfloat *values)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alSourcei(ALuint sourceId, ALenum param, ALint value)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		case AL_LOOPING:
			source->looping = value;
		break;
		case AL_SOURCE_STATE:
			assert(value==AL_INITIAL || value==AL_PAUSED || value==AL_STOPPED || value==AL_PLAYING);
			source->sourceState = value;
		break;		
		default:
		break;
	};
}
	
AL_API ALvoid AL_APIENTRY alSource3i(ALuint sourceId, ALenum param, ALint value1, ALint value2, ALint value3)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alSourceiv(ALuint sourceId, ALenum param, const ALint *values)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alGetSourcef(ALuint sourceId, ALenum param, ALfloat *value)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		case AL_PITCH:
			*value = source->pitch;
		break;
		case AL_GAIN:
			*value = source->gain;
		break;
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alGetSource3f(ALuint sourceId, ALenum param, ALfloat *value1, ALfloat *value2, ALfloat *value3)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alGetSourcefv(ALuint sourceId, ALenum param, ALfloat *values)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alGetSourcei(ALuint sourceId,  ALenum param, ALint *value)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	assert(source);
	
	switch(param)
	{
		case AL_LOOPING:
			*value = source->looping;
		break;
		case AL_SOURCE_STATE:
			*value = source->sourceState;
		break;
		case AL_BUFFERS_PROCESSED:
			*value = source->processed;
		break;
		case AL_BUFFERS_QUEUED:
			if(source->queueTail >= source->queueHead)
				*value = source->queueTail - source->queueHead;
			else
				*value = (source->queueTail) + (SOURCE_QUEUE_MAX - source->queueHead);
		break;
		
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alGetSource3i(ALuint sourceId, ALenum param, ALint *value1, ALint *value2, ALint *value3)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alGetSourceiv(ALuint sourceId, ALenum param, ALint *values)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(!source)
		return;
	
	switch(param)
	{
		default:
		break;
	};
}

AL_API ALvoid AL_APIENTRY alGenSources(ALsizei n, ALuint *sourceIDs)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	while(n--)
	{
		if(!ctx)
		{
			*sourceIDs++ = 0;
		}
		else
		{
			source = (struct ALSource*)malloc(sizeof(struct ALSource));
			memset(source, 0, sizeof(struct ALSource));
			source->id = ++alSourceGUID;
			source->looping = FALSE;
			source->gain = 1;
			source->pitch = 1;
			source->sourceState = AL_INITIAL;
			memset(source->queue, 0, SOURCE_QUEUE_MAX*sizeof(source->queue[0]));
			source->queueHead = 0;
			source->queueTail = 0;
			source->processed = 0;
			source->prev = 0;
			source->next = 0;
			
			if(ctx->headSource==0)
			{
				source->prev = 0;
				source->next = 0;
				ctx->headSource = source;
				ctx->tailSource = source;
			}
			else
			{
				source->prev = ((struct ALSource*)ctx->tailSource);
				((struct ALSource*)ctx->tailSource)->next = source;
			
				ctx->tailSource = (void *)source;
			}
					
			*sourceIDs++ = source->id;
		}
	}
}

AL_API ALvoid AL_APIENTRY alDeleteSources(ALsizei n, const ALuint *sourceIDs)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	while(n--)
	{
		source = alFindSource(*sourceIDs);
		if(source)
		{
			if(source->prev)
			{
				if(source->next)
				{
					source->prev->next = source->next;
					source->next->prev = source->prev;
				}
				else
				{
					source->prev->next = 0;
					
					ctx->tailSource = source->prev;
				}
			}
			else
			{
				if(source->next)
				{
					source->next->prev = 0;
					
					ctx->headSource = source->next;
				}
				else
				{
					ctx->headSource = 0;
					ctx->tailSource = 0;
				}				
			}
			
			ctx->lastSource = 0;
			free(source);
		}
		
		sourceIDs++;
	}	
}

AL_API ALboolean AL_APIENTRY alIsSource(ALuint sourceId)
{
	return alFindSource(sourceId)!=0;
}

AL_API ALvoid AL_APIENTRY alGenBuffers(ALsizei n, ALuint *bufferIDs)
{
	struct ALBuffer* buffer;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	while(n--)
	{
		if(!ctx)
		{
			*bufferIDs++ = 0;
		}
		else
		{
			buffer = (struct ALBuffer*)malloc(sizeof(struct ALBuffer));
			memset(buffer, 0, sizeof(struct ALBuffer));
			
			buffer->id = ++alBufferGUID;
			buffer->freq = 0;
			buffer->format = AL_FORMAT_STEREO16;
			buffer->buf = 0;
			buffer->ridx = 0;			
			buffer->bufSize = 0;
			buffer->prev = 0;
			buffer->next = 0;

			if(ctx->headBuffer==0)
			{
				ctx->headBuffer = buffer;
				ctx->tailBuffer = buffer;
			}
			else
			{
				buffer->prev = ((struct ALBuffer*)ctx->tailBuffer);
				((struct ALBuffer*)ctx->tailBuffer)->next = buffer;
			
				ctx->tailBuffer = (void *)buffer;
			}
		
			*bufferIDs++ = buffer->id;
		}
	}
}

AL_API ALvoid AL_APIENTRY alDeleteBuffers(ALsizei n, const ALuint *bufferIDs)
{
	struct ALBuffer* buffer;	
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	while(n--)
	{
		buffer = alFindBuffer(*bufferIDs);
		if(buffer)
		{
			if(buffer->prev)
			{
				if(buffer->next)
				{
					buffer->prev->next = buffer->next;
					buffer->next->prev = buffer->prev;
				}
				else
				{
					buffer->prev->next = 0;
					
					ctx->tailBuffer = buffer->prev;
				}
			}
			else
			{
				if(buffer->next)
				{
					buffer->next->prev = 0;
					
					ctx->headBuffer = buffer->next;
				}
				else
				{
					ctx->headBuffer = 0;
					ctx->tailBuffer = 0;
				}				
			}
			
			ctx->lastBuffer = 0;
			free(buffer);
		}
		
		bufferIDs++;
	}	
}

AL_API ALboolean AL_APIENTRY alIsBuffer(ALuint bufferId)
{
	return alFindBuffer(bufferId)!=0;
}

AL_API ALvoid AL_APIENTRY alSourcePlay(ALuint sourceId)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(source->sourceState != AL_PLAYING)
	{
		source->sourceState = AL_PLAYING;
#ifdef STM32	
		TIMER_Start();
#else
#endif
	}
}

AL_API ALvoid AL_APIENTRY alSourceStop(ALuint sourceId)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(source->sourceState != AL_STOPPED)
	{
		source->sourceState = AL_STOPPED;
#ifdef STM32
		TIMER_Stop();
#else
#endif
	}
}

AL_API ALvoid AL_APIENTRY alSourceRewind(ALuint sourceId)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(source->sourceState != AL_INITIAL)
	{
		source->sourceState = AL_STOPPED;
#ifdef STM32
		TIMER_Stop();
#else	
#endif
	}
}


AL_API ALvoid AL_APIENTRY alSourcePause(ALuint sourceId)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(source->sourceState != AL_PAUSED)
	{
		source->sourceState = AL_PAUSED;
#ifdef STM32
		TIMER_Stop();
#else
#endif
	}
}

AL_API ALvoid AL_APIENTRY alSourceResume(ALuint sourceId)
{
	struct ALSource* source;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	if(source->sourceState == AL_PAUSED)
	{
		source->sourceState = AL_PLAYING;
#ifdef STM32	
		TIMER_Start();
#else
#endif	
	}
}

AL_API ALvoid AL_APIENTRY alSourceQueueBuffers(ALuint sourceId, ALsizei nb, const ALuint *bufferIds)
{
	struct ALSource* source;
	struct ALBuffer* buffer;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	source = alFindSource(sourceId);
	assert(source);
	while(nb--)
	{
		if((source->queueTail+1) % SOURCE_QUEUE_MAX==source->queueHead)
		{
			ctx->error = AL_INVALID_OPERATION; // buffer overflow
			return;
		}
		buffer = alFindBuffer(*bufferIds++);
		buffer->ridx = 0;		
		source->queue[source->queueTail] = buffer;
		source->queueTail = (source->queueTail+1) % SOURCE_QUEUE_MAX;
	}
}

void mutex()
{
	//while(TIM_GetITStatus(TIM3, TIM_IT_Update)==SET);
}

AL_API ALvoid AL_APIENTRY alSourceUnqueueBuffers(ALuint sourceId, ALsizei nb, ALuint *bufferIds)
{
	struct ALSource* source;
	struct ALBuffer* buffer;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);

#ifdef STM32
	source = alFindSource(sourceId);
	assert(source);
	while(nb--)
	{
		if(source->queueHead==source->queueTail)
		{
			//ctx->error = AL_INVALID_OPERATION; // buffer empty
			return;
		}
		
		*bufferIds = source->queue[source->queueHead]->id;
		source->queue[source->queueHead] = 0;
		source->queueHead = (source->queueHead+1) % SOURCE_QUEUE_MAX;
		
		source->processed--;		
	}
#else	
#endif
}

AL_API ALvoid AL_APIENTRY alSourceUpdate(void* head)
{
#ifdef STM32
	struct ALSource* source = (struct ALSource* )head;
	struct ALBuffer* buffer;
	s16* l;
	s16* r;
	u32 nextIdx;
	if(source->queueHead==source->queueTail)
		return;
	
	buffer = source->queue[source->queueHead];
	if(!buffer)
		return;
		
	l = (s16*)(buffer->buf + (buffer->ridx<<1));
	r = l + 1;

	buffer->ridx += 4;
	TIM3->CCR3 = (u16)((*l)>>6);
	TIM3->CCR4 = (u16)((*r)>>6);
	//TIM_SetCompare3(TIM3, (u16)((*l + 32768)>>6));
	// TIM_SetCompare4(TIM3, (u16)((*r + 32768)>>6));
	
	if(buffer->ridx>=buffer->bufSize)
	{
		buffer->ridx = 0;
		nextIdx = (source->queueHead+1) % SOURCE_QUEUE_MAX;
		if(nextIdx!=source->queueTail)
		{
			source->processed++;
		}
	}
#else	
#endif
}

AL_API ALvoid AL_APIENTRY alBufferData(ALuint bufferId, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq)
{
	struct ALBuffer* buffer;
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	buffer = alFindBuffer(bufferId);
	if(buffer)
	{
		buffer->freq = freq;
		buffer->format = format;
		if(!buffer->buf)
		{
			buffer->buf = (u8*)malloc(size);
			buffer->bufSize = size;
		}
		else
		{
			if(buffer->bufSize != size)
			{
				free(buffer->buf);
				buffer->buf = (u8*)malloc(size);
				buffer->bufSize = size;
			}
		}
		memcpy(buffer->buf, data, buffer->bufSize);
	}
}

AL_API ALenum AL_APIENTRY alGetError(void)
{
	ALCcontext* ctx = alcGetContextCurrent();
	assert(ctx);
	
	return ctx->error;
}

