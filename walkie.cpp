#include "pxt.h"
#include "MicroBit.h"
#include "StreamRecording.h"

using namespace pxt;

namespace WalkieTalkie {
    StreamRecording* recording = nullptr;
    SplitterChannel* channel = nullptr;
    const int SAMPLE_RATE = 8000;          // Lower sample rate saves RAM
    const int BUFFER_LEN = SAMPLE_RATE * 3; // 3 seconds max record duration

    //% block="Initialize Walkie Talkie"
    void init() {
        if (recording != nullptr) return;
        
        // Connect to internal microphone stream
        channel = uBit.audio.splitter->createChannel();
        channel->requestSampleRate(SAMPLE_RATE);
        recording = new StreamRecording(*channel, BUFFER_LEN);
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
        
        // Make sure it stops recording data before extracting
        recording->stop();

        int bytesRemaining = recording->length();
        int offset = 0;
        uint8_t packet[32]; // Maximum 2.4Ghz radio packet payload size

        // Read and loop through the buffer chunk by chunk
        while (bytesRemaining > 0) {
            int chunkSize = (bytesRemaining > 32) ? 32 : bytesRemaining;
            
            // Extract a 32-byte window from raw audio buffer
            recording->read(packet, offset, chunkSize);
            
            // Broadcast packet over the raw datagram layer
            uBit.radio.datagram.send(packet, chunkSize);
            
            offset += chunkSize;
            bytesRemaining -= chunkSize;
            
            // Small pause so the wireless pipeline does not lock up or drop packets
            fiber_sleep(10); 
        }
    }
}
