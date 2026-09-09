//% color="#E67E22" icon="\uf130" block="Walkie Talkie"
namespace walkieTalkie {

    //% block="initialize walkie talkie"
    //% shim=WalkieTalkie::init
    export function init(): void {
        return; 
    }

    //% block="set walkie talkie group to %group"
    //% group.min=0 group.max=255 group.defl=1
    //% shim=WalkieTalkie::setRadioGroup
    export function setRadioGroup(group: number): void {
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
