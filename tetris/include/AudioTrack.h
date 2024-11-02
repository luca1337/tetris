#pragma once

#include <Logger.h>
#include <IResource.h>

#include <SDL2/SDL_mixer.h>

#include <string>
#include <iostream>

class AudioTrack : public IResource
{
public:
    ~AudioTrack();
    AudioTrack(const std::string& path);
    AudioTrack(const AudioTrack& other) = default;

    auto Play(const bool loop = false) -> void;
    auto Pause() -> void;
    auto Resume() -> void;
    auto Stop() -> void;
    auto IsPlaying() const { return Mix_Playing(m_Channel) > 0; }

private:
    Mix_Chunk* m_Chunk = {};
    int m_Channel = {};
};
