/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#include "AudioStream.hpp"

#include "../Logger.hpp"

namespace NerdThings::Ngine::Audio {
    void AudioStream::UpdateStream(const void *data_, int samplesCount_) {
        if (Buffer != nullptr) {
            if (Buffer->IsSubBufferProcessed[0] || Buffer->IsSubBufferProcessed[1]) {
                ma_uint32 subBufferToUpdate = 0;

                if (Buffer->IsSubBufferProcessed[0] && Buffer->IsSubBufferProcessed[1]) {
                    subBufferToUpdate = 0;
                    Buffer->FrameCursorPos = 0;
                } else {
                    subBufferToUpdate = (Buffer->IsSubBufferProcessed[0]) ? 0 : 1;
                }

                ma_uint32 subBufferSizeInFrames = Buffer->BufferSizeInFrames / 2;
                unsigned char *subBuffer = (unsigned char*)Buffer->Buffer + ((subBufferSizeInFrames*Channels*(SampleSize/8))*subBufferToUpdate);

                Buffer->TotalFramesProcessed += subBufferSizeInFrames;

                if (subBufferSizeInFrames >= (ma_uint32)samplesCount_/Channels) {
                    ma_uint32 framesToWrite = subBufferSizeInFrames;

                    if (framesToWrite > ((ma_uint32) samplesCount_ / Channels))
                        framesToWrite = (ma_uint32) samplesCount_ / Channels;

                    ma_uint32 bytesToWrite = framesToWrite * Channels * (SampleSize / 8);
                    memcpy(subBuffer, data_, bytesToWrite);

                    // Any leftover, fill with zero
                    ma_uint32 leftoverFrameCount = subBufferSizeInFrames - framesToWrite;

                    if (leftoverFrameCount > 0)
                        memset(subBuffer + bytesToWrite, 0, leftoverFrameCount * Channels * (SampleSize / 8));

                    Buffer->IsSubBufferProcessed[subBufferToUpdate] = false;
                } else Logger::Error("AudioStream", "Attempting to write too many frames to buffer.");
            } else Logger::Error("AudioStream", "Audio buffer not available for updating.");
        } else Logger::Error("AudioStream", "No audio buffer.");
    }
}