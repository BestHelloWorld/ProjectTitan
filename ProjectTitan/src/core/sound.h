#ifndef __SOUND_H
#define __SOUND_H

extern "C"
{
#include "al.h"
#include "alc.h"
#include "vorbis/vorbisfile.h"
}

#include <vector>
#include <map>

#define BUFFER_SIZE   32768     // 32 KB buffers
void LoadOGG(const char * file_name, std::vector<char> & buffer, ALenum & format, ALsizei & frequency);


typedef struct tagPOSITION
{
	float x;
	float y;
	float z;
}POSITION;

#define PL_LISTENER		0x1101
#define PL_SOURCES		0x1102

#define PL_PITCH		0x2101
#define PL_GAIN			0x2102

#define PL_CHECK_RATE	16

class Sound
{
public:
	void Init();
	void Release();

	// LOAD
	void LoadAudio(const char * ogg_file, unsigned int * audio_id);
	void UnloadAudio(unsigned int audio_id);

	// FUNCTION
	/* More than 0 is the number of times, Less than 0 is the infinite loop. */
	void Play(unsigned int audio_id, int count = 1);
	void PlayRewind(unsigned int audio_id);
	void Pause(unsigned int audio_id);
	void Stop(unsigned int audio_id);

	// PROPERTIES
	//void SetSourcei(unsigned int type, unsigned int audio_id, int value);
	void SetSourcef(unsigned int type, unsigned int audio_id, float value);

	// OTHER
	unsigned int GetBuffer(unsigned int audio_id);

	// POSITION
	void SetPosition(unsigned int type, float x, float y, float z, unsigned int audio_id = 0);
	void Move(unsigned int type, float x, float y, float z, unsigned int audio_id = 0);
	POSITION GetPosition(unsigned int type, unsigned int audio_id = 0);

	Sound * GetInstance()
	{
		return mPool;
	}

private:
	void PlayLoop(unsigned int audio_id, int count);
	void StopLoop(unsigned int audio_id);

	static Sound * mPool;

	ALCdevice * mDevice = NULL;
	ALCcontext * mContext = NULL;

	std::vector<unsigned int> mSounds;

	POSITION mListenerPos;
	typedef std::pair<unsigned int, POSITION*> SoundPosPair;
	std::map<unsigned int, POSITION*> mSoundsPos;

	typedef std::pair<unsigned int, int> SoundLoopPlayPair;
	std::map<unsigned int, int> mLoopCount;
};

#endif