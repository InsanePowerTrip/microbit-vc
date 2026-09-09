#include "pxt.h"
#include "MicroBit.h"
#include "StreamRecording.h"

using namespace pxt;

namespace WalkieTalkie {
    StreamRecording* recording = nullptr;
    SplitterChannel* channel = nullptr;
    int currentSampleRate = 8000; // Default fallback

    //% block="Initialize Walkie Talkie with sample rate %sampleRate Hz"
    //% sampleRate.defl=8000
    void init(int sampleRate) {
        // Clear old instances if initializing again with a different rate
        if (recording != nullptr) {
            delete recording;
            recording = nullptr;
        }

        currentSampleRate = sampleRate;
        
        // 3-second buffer calculation (Sample Rate * Seconds)
        int bufferLen = currentSampleRate * 3; 

        // Connect to internal microphone stream
        channel = uBit.audio.splitter->createChannel();
        channel->requestSampleRate(currentSampleRate);
        recording = new StreamRecording(*channel, bufferLen);

        // Safely wake up the radio layer using default/existing settings
        uBit.radio.enable();
    }

    //% block="Start Recording"
    void startRecording() {
        if (!recording) return;
        recording->erase();
        recording->recordAsync();
    }

    //% block="Transmit Recorded Audio"
    void transmitAudio() {
        if (!recording) return;
        
        recording->stop();

        int bytesRemaining = recording->length();
        int offset = 0;
        uint8_t packet; // 32-byte hardware payload window

        while (bytesRemaining > 0) {
            int chunkSize = (bytesRemaining > 32) ? 32 : bytesRemaining;
            
            recording->read(packet, offset, chunkSize);
            
            // Transmits via whatever group was set globally in MakeCode [^1]
            uBit.radio.datagram.send(packet, chunkSize);
            
            offset += chunkSize;
            bytesRemaining -= chunkSize;
            
            fiber_sleep(10); 
        }
    }
}
