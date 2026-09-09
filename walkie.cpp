#include "pxt.h"
#include "MicroBit.h"
#include "StreamRecording.h"

using namespace pxt;

namespace WalkieTalkie {
    StreamRecording* recording = nullptr;
    SplitterChannel* channel = nullptr;
    const int SAMPLE_RATE = 8000;          
    const int BUFFER_LEN = SAMPLE_RATE * 3; 

    //% block="Initialize Walkie Talkie"
    void init() {
        if (recording != nullptr) return;
        
        channel = uBit.audio.splitter->createChannel();
        channel->requestSampleRate(SAMPLE_RATE);
        recording = new StreamRecording(*channel, BUFFER_LEN);

        // Turn on the micro:bit radio system safely
        uBit.radio.enable();
    }

    //% block="Set Walkie Talkie Group to %group"
    //% group.min=0 group.max=255 group.defl=1
    void setRadioGroup(int group) {
        // Direct CODAL hardware override for the channel group
        uBit.radio.setGroup(group);
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
        uint8_t packet[32]; 

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
