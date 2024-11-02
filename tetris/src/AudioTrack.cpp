#include <AudioTrack.h>

AudioTrack::~AudioTrack()
{
    Mix_FreeChunk(m_Chunk);
    Mix_CloseAudio();
}

AudioTrack::AudioTrack(const std::string &path)
{
    const auto flags = MIX_INIT_OGG | MIX_INIT_FLAC;
    const auto initted = Mix_Init(flags);
    if ((initted & flags) != flags) 
    {
        printf("Mix_Init: Failed to init required ogg and mod support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
    }

    m_Chunk = Mix_LoadWAV(path.c_str());
    if(!m_Chunk) 
    {
        printf("Mix_LoadMUS(\"music.mp3\"): %s\n", Mix_GetError());
        return;
    }
}

auto AudioTrack::Play(const bool loop) -> void 
{
    m_Channel = Mix_PlayChannel(-1, m_Chunk, loop ? -1 : 0);
}

auto AudioTrack::Pause() -> void 
{
    Mix_Pause(m_Channel);
}

auto AudioTrack::Resume() -> void 
{
    Mix_Resume(m_Channel);
}

auto AudioTrack::Stop() -> void 
{
    Mix_HaltChannel(m_Channel);
}
