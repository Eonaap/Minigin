#include "MiniginPCH.h"
#include "AudioManager.h"
#include <cassert>

int kaas::AudioManager::m_Head = 0;
int kaas::AudioManager::m_Tail = 0;
int kaas::AudioManager::m_NumPending = 0;
std::vector<PlayMessage> kaas::AudioManager::m_PendingList = std::vector<PlayMessage>();

kaas::AudioManager::AudioManager()
{
	m_PendingList.reserve(m_MaxRequests);
	for (size_t i = 0; i < m_MaxRequests; i++)
	{
		m_PendingList.push_back(PlayMessage{});
	}
}

kaas::AudioManager::~AudioManager()
{
	//Free the music
	for (AudioPiece music : m_pAudioList)
	{
		Mix_FreeChunk(music.audio);
		music.audio = NULL;
	}
}

void kaas::AudioManager::PlaySound(int soundID, int volume, int loops)
{
	//Play the music
	if (!m_pAudioList[soundID].playing) {

		m_pAudioList[soundID].channel == 0 ? m_pAudioList[soundID].channel = Mix_PlayChannel(-1, m_pAudioList[soundID].audio, loops) 
											: Mix_PlayChannel(m_pAudioList[soundID].channel, m_pAudioList[soundID].audio, loops);
		
		m_pAudioList[soundID].playing = true;

		Mix_Volume(m_pAudioList[soundID].channel, volume);
	}
}

void kaas::AudioManager::StopSound(int soundID)
{
	// Stop sound using console audio api...
	UNREFERENCED_PARAMETER(soundID);
	if (m_pAudioList[soundID].playing)
	{
		Mix_HaltChannel(soundID);
		m_pAudioList[soundID].channel = 0;
		m_pAudioList[soundID].playing = false;
	}
}

void kaas::AudioManager::AddSound(std::string soundFilePath)
{
	//Load music
	Mix_Chunk* sound = Mix_LoadWAV(soundFilePath.c_str());
	if (sound == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	m_pAudioList.push_back(AudioPiece{ sound, 0, false });
}

void kaas::AudioManager::Update()
{
	// If there are no pending requests, do nothing.
	if (m_Head == m_Tail) return;

	PlaySound(m_PendingList[m_Head].soundID, m_PendingList[m_Head].volume, m_PendingList[m_Head].loops);

	m_Head = (m_Head + 1) % m_MaxRequests;
}

void kaas::AudioManager::AddRequest(int soundID, int volume, int loops)
{
	if ((m_Tail + 1) % m_MaxRequests == m_Head)
		return;

	// Add to the end of the list.
	m_PendingList[m_Head].soundID = soundID;
	m_PendingList[m_Head].volume = volume;
	m_PendingList[m_Head].loops = loops;
	m_Tail = (m_Tail + 1) % m_MaxRequests;
}
