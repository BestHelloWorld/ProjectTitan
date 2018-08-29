#include "sound.h"

#include <thread>
#include <chrono>
#include <cstdlib>


void LoadOGG(const char * file_name, std::vector<char> & buffer, ALenum & format, ALsizei & frequency)
{
	int endian = 0;
	int bitStream;
	long len = 0;
	char buf[BUFFER_SIZE];
	FILE * fp;
	fp = fopen(file_name, "rb");

	vorbis_info * pInfo;
	OggVorbis_File oggFile;

	ov_open(fp, &oggFile, NULL, 0);
	pInfo = ov_info(&oggFile, 0);

	if (!pInfo)
		return;

	if (pInfo->channels == 1)
	{
		format = AL_FORMAT_MONO16;
	}
	else
	{
		format = AL_FORMAT_STEREO16;
	}

	frequency = pInfo->rate;

	do
	{
		len = ov_read(&oggFile, buf, BUFFER_SIZE, endian, 2, 1, &bitStream);
		buffer.insert(buffer.end(), buf, buf + len);
	} while (len > 0);

	ov_clear(&oggFile);
}

Sound * Sound::mPool = new Sound;

void Sound::Init()
{
	if (mDevice && mContext)
		return;

	mDevice = alcOpenDevice(NULL);
	mContext = alcCreateContext(mDevice, NULL);
	alcMakeContextCurrent(mContext);

	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
}

void Sound::Release()
{
	auto iter = mSounds.begin();
	while (iter != mSounds.end())
	{
		int source = *iter._Ptr;
		int buffer = 0;
		alGetSourcei(source, AL_BUFFER, &buffer);
		alDeleteSources(1, (unsigned int*)&source);
		alDeleteBuffers(1, (unsigned int*)&buffer);

		iter++;
	}

	mDevice = alcGetContextsDevice(mContext);
	alcDestroyContext(mContext);
	alcCloseDevice(mDevice);
}

void Sound::LoadAudio(const char * ogg_file, unsigned int * source_id)
{
	unsigned int buffer = 0;
	unsigned int source = 0;
	alGenBuffers(1, &buffer);
	alGenSources(1, &source);

	std::vector<char> arrays;
	ALenum type;
	int frequency;
	LoadOGG(ogg_file, arrays, type, frequency);

	alBufferData(buffer, type, &arrays[0], arrays.size(), frequency);
	alSourcei(source, AL_BUFFER, buffer);

	alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);

	*source_id = source;

	mSoundsPos.insert(SoundPosPair(source, new POSITION{ 0.0f, 0.0f, 0.0f }));
	mSounds.push_back(source);

	arrays.clear();
}

void Sound::UnloadAudio(unsigned int audio_id)
{
	int buffer = 0;
	alGetSourcei(audio_id, AL_BUFFER, &buffer);
	alDeleteBuffers(1, (unsigned int *)&buffer);
	alDeleteSources(1, &audio_id);

	for (auto iter = mSounds.begin(); iter != mSounds.end(); iter++)
	{
		if (audio_id == iter[0])
		{
			mSounds.erase(iter);
			break;
		}
	}

	printf("Erase source id : %d, current loaded audio size : %d\n", audio_id, mSounds.size());
}

void Sound::Play(unsigned int audio_id, int count)
{
	//int state;
	//alGetSourcei(audio_id, AL_SOURCE_STATE, &state);
	//if (state == AL_PLAYING)
	//	alSourcePause(audio_id);
	alSourcePlay(audio_id);

	if (count == 1)
		return;

	PlayLoop(audio_id, --count);
}

void Sound::PlayRewind(unsigned int audio_id)
{
	int state;
	alGetSourcei(audio_id, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING)
		alSourceRewind(audio_id);
}

void Sound::Pause(unsigned int audio_id)
{
	int state;
	alGetSourcei(audio_id, AL_SOURCE_STATE, &state);
	if (state == AL_PAUSED)
		return;

	alSourcePause(audio_id);
}

void Sound::Stop(unsigned int audio_id)
{
	int state;
	alGetSourcei(audio_id, AL_SOURCE_STATE, &state);
	if (state == AL_STOPPED)
		return;

	alSourceStop(audio_id);
	StopLoop(audio_id);
}

void Sound::SetSourcef(unsigned int type, unsigned int audio_id, float value)
{
	switch (type)
	{
	case PL_PITCH:
	{
		alSourcef(audio_id, AL_PITCH, value);
		break;
	}
	case PL_GAIN:
	{
		//alSourcef(audio_id, AL_SPEED_OF_SOUND, value);
		alSourcef(audio_id, AL_GAIN, value);
		break;
	}
	default:
		break;
	}
}

unsigned int Sound::GetBuffer(unsigned int audio_id)
{
	unsigned int buffer = 0;
	alGetSourcei(audio_id, AL_BUFFER, (int*)&buffer);
	return buffer;
}

void Sound::SetPosition(unsigned int type, float x, float y, float z, unsigned int audio_id)
{
	switch (type)
	{
	case PL_LISTENER:
		alListener3f(AL_POSITION, x, y, z);
		break;
	case PL_SOURCES:
		alSource3f(audio_id, AL_POSITION, x, y, z);
		break;
	}
}

void Sound::Move(unsigned int type, float x, float y, float z, unsigned int audio_id)
{
	POSITION pos;
	alGetSource3f(audio_id, AL_POSITION, &pos.x, &pos.y, &pos.z);
	pos.x += x;
	pos.y += y;
	pos.z += z;
	SetPosition(type, pos.x, pos.y, pos.z, audio_id);
}

POSITION Sound::GetPosition(unsigned int type, unsigned int audio_id)
{
	POSITION res = { 0 };
	switch (type)
	{
	case PL_LISTENER:
		alGetListener3f(AL_POSITION, &res.x, &res.y, &res.z);
		break;
	case PL_SOURCES:
		alGetSource3f(audio_id, AL_POSITION, &res.x, &res.y, &res.z);
		break;
	}
	return res;
}

void Sound::PlayLoop(unsigned int audio_id, int count)
{
	if (!count)
		return;

	auto iter = mLoopCount.find(audio_id);
	if (iter == mLoopCount.end())
	{
		iter = mLoopCount.insert(SoundLoopPlayPair(audio_id, count)).first;
	}
	iter->second = count;

	std::thread t([=](int * count)->void {
		int state = 0;
		while (*count)
		{
			while (1)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(PL_CHECK_RATE));

				alGetSourcei(audio_id, AL_SOURCE_STATE, &state);
				if (!state)
					return;

				switch (state)
				{
				case AL_STOPPED:
					alSourcePlay(audio_id);
					goto _subcount;
				case AL_PLAYING:
				case AL_PAUSED:
					break;
				}
			}
		_subcount:
			if (*count < 0)
				continue;
			else if (*count > 0)
				--*count;
		}

		printf("thread was out");
	}, &iter->second);
	t.detach();

}

void Sound::StopLoop(unsigned int audio_id)
{
	auto iter = mLoopCount.find(audio_id);
	if (iter == mLoopCount.end())
	{
		return;
	}

	printf("%d\n", iter->second);

	iter->second = 0;
}