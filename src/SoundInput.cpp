#include "SoundInput.hpp"
#include <cmath>

SoundInput::SoundInput()
: volume(0.f), threshold(0.02f)
{
    Pa_Initialize();
    Pa_OpenDefaultStream(&stream, 1, 0, paFloat32,
                         SAMPLE_RATE, FRAMES_PER_BUFFER,
                         paCallback, this);
    Pa_StartStream(stream);
}

SoundInput::~SoundInput() {
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
}

int SoundInput::paCallback(const void* inputBuffer, void*,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo*,
                           PaStreamCallbackFlags, void* userData)
{
    SoundInput* self = static_cast<SoundInput*>(userData);
    const float* in = static_cast<const float*>(inputBuffer);
    float sum = 0.f;
    for (unsigned long i = 0; i < framesPerBuffer; ++i)
        sum += in[i] * in[i];
    self->volume = std::sqrt(sum / framesPerBuffer);
    return paContinue;
}

float SoundInput::getVolume() const {
    return volume;
}

float SoundInput::getThreshold() const {
    return threshold;
}
