//% color="#E67E22" icon="\uf130" block="Walkie Talkie"
namespace walkieTalkie {

    /**
     * Initialize walkie talkie with a completely custom sample rate in Hz.
     * @param sampleRate Choose any rate (e.g., 4000 to 11000), default: 8000
     */
    //% block="initialize walkie talkie with sample rate $sampleRate Hz"
    //% sampleRate.defl=8000
    //% shim=WalkieTalkie::init
    export function init(sampleRate: number): void {
        return; 
    }

    //% block="start recording audio"
    //% shim=WalkieTalkie::startRecording
    export function startRecording(): void {
        return;
    }

    //% block="transmit recorded audio"
    //% shim=WalkieTalkie::transmitAudio
    export function transmitAudio(): void {
        return;
    }
}
