#include "pxt.h"
#include "MicroBit.h"
#include "StreamRecording.h"

using namespace pxt;

namespace WalkieTalkie {
    StreamRecording* recording = nullptr;
    SplitterChannel* channel = nullptr;

    void init(int sampleRate) {
        // Clear previous configurations if re-initialized
        if (recording != nullptr) {
            delete recording;
            recording = nullptr;
        }

        // Connect raw hardware pipeline directly to the typed value
        channel = uBit.audio.splitter->createChannel();
        channel->requestSampleRate(sampleRate);
        
        // Dynamically compute exact RAM buffer footprint (Sample Rate * 3 seconds)
        int bufferLen = sampleRate * 3; 
        recording = new StreamRecording(*channel, bufferLen);

        uBit.radio.enable();
    }

    void startRecording() {
        if (!recording) return;
        recording->erase();
        recording->recordAsync();
    }

    void transmitAudio() {
        if (!recording) return;
        
        recording->stop();

        int bytesRemaining = recording->length();
        int offset = 0;
        uint8_t packet; 

        while (bytesRemaining > 0) {
            int chunkSize = (bytesRemaining > 32) ? 32 : bytesRemaining;
            recording->read(packet, offset, chunkSize);
            uBit.radio.datagram.send(packet, chunkSize);
            
            offset += chunkSize;
            bytesRemaining -= chunkSize;
            fiber_sleep(10); 
        }
    }
}
