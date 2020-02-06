/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#include "audio/AudioStream.hpp"
#include "Console.hpp"

#include <cstring>

namespace ngine::audio {
    void AudioStream::updateStream(const void *data_, int samplesCount_) {
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
                } else Console::Error("AudioStream", "Attempting to write too many frames to buffer.");
            } else Console::Error("AudioStream", "Audio buffer not available for updating.");
        } else Console::Error("AudioStream", "No audio buffer.");
    }
}