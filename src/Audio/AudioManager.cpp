/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "AudioManager.h"

namespace NerdThings::Ngine::Audio {
    // Private Fields

    std::vector<std::shared_ptr<TMusic>> AudioManager::_ActiveMusic;
    bool AudioManager::_Initialized = false;

    // Public Methods

    void AudioManager::CloseDevice() {
        //CloseAudioDevice();
    }

    float AudioManager::GetMusicLength(std::shared_ptr<TMusic> mus_) {
        //return GetMusicTimeLength(mus_->ToRaylibMusic());
        return 0;
    }

    float AudioManager::GetMusicTimePlayed(std::shared_ptr<TMusic> mus_) {
        //return ::GetMusicTimePlayed(mus_->ToRaylibMusic());
        return 0;
    }

    void AudioManager::InitDevice() {
        //InitAudioDevice();
    }

    bool AudioManager::IsPlaying(std::shared_ptr<TMusic> mus_) {
        //return IsMusicPlaying(mus_->ToRaylibMusic());
        return false;
    }

    bool AudioManager::IsPlaying(std::shared_ptr<TSound> snd_) {
        //return IsSoundPlaying(snd_->ToRaylibSound());
        return false;
    }

    bool AudioManager::IsReady() {
        //return IsAudioDeviceReady();
        return false;
    }

    void AudioManager::Pause(std::shared_ptr<TMusic> mus_) {
        //PauseMusicStream(mus_->ToRaylibMusic());
    }

    void AudioManager::Pause(std::shared_ptr<TSound> snd_) {
        //PauseSound(snd_->ToRaylibSound());
    }

    void AudioManager::Play(std::shared_ptr<TMusic> mus_) {
        //if (std::find(_ActiveMusic.begin(), _ActiveMusic.end(), mus_) == _ActiveMusic.end())
        //    _ActiveMusic.push_back(mus_);
        //PlayMusicStream(mus_->ToRaylibMusic());
    }

    void AudioManager::Play(std::shared_ptr<TSound> snd_) {
        //PlaySound(snd_->ToRaylibSound());
    }

    void AudioManager::SetMasterVolume(float vol_) {
        //::SetMasterVolume(vol_);
    }

    void AudioManager::SetLoopCount(std::shared_ptr<TMusic> mus_, int loops_) {
        //SetMusicLoopCount(mus_->ToRaylibMusic(), loops_);
    }

    void AudioManager::SetPitch(std::shared_ptr<TMusic> mus_, float pitch_) {
        //SetMusicPitch(mus_->ToRaylibMusic(), pitch_);
    }

    void AudioManager::SetPitch(std::shared_ptr<TSound> snd_, float pitch_) {
        //SetSoundPitch(snd_->ToRaylibSound(), pitch_);
    }

    void AudioManager::SetVolume(std::shared_ptr<TMusic> mus_, float vol_) {
        //SetMusicVolume(mus_->ToRaylibMusic(), vol_);
    }

    void AudioManager::SetVolume(std::shared_ptr<TSound> snd_, float vol_) {
        //SetSoundVolume(snd_->ToRaylibSound(), vol_);
    }
    
    void AudioManager::Stop(std::shared_ptr<TMusic> mus_) {
        //StopMusicStream(mus_->ToRaylibMusic());
        //_ActiveMusic.erase(std::remove(_ActiveMusic.begin(), _ActiveMusic.end(), mus_), _ActiveMusic.end());
    }

    void AudioManager::Stop(std::shared_ptr<TSound> snd_) {
        //StopSound(snd_->ToRaylibSound());
    }

    void AudioManager::Update(EventArgs &e) {
        //for (auto music : _ActiveMusic) {
        //    music->Update();
        //}
    }
}
