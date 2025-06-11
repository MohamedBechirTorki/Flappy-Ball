#ifndef SOUNDINPUT_HPP
#define SOUNDINPUT_HPP

#include <portaudio.h>    // <-- use angle brackets

class SoundInput {
public:
    SoundInput();
    ~SoundInput();
    float getVolume() const;
    float getThreshold() const;

private:
    PaStream* stream;
    static constexpr int SAMPLE_RATE = 44100;
    static constexpr int FRAMES_PER_BUFFER = 512;
    float volume;
    float threshold;
    static int paCallback(const void* inputBuffer, void* /*outputBuffer*/,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* /*timeInfo*/,
                          PaStreamCallbackFlags /*statusFlags*/,
                          void* userData);
};

#endif // SOUNDINPUT_HPP
