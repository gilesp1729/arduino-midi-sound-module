# Arduino MIDI Sound Library
This is a fork of [Arduino MIDI Sound Module](https://github.com/DLehenbauer/arduino-midi-sound-module) with the following changes:
* MIDI messages are not being received on the USART. The underlying routines for starting notes, etc. are exposed so that calling programs may call them directly. They still follow the MIDI functions closely. A sample test program is provided.
* Arduino routines like Serial are available for use. Timer 0 is still used for PWM output so delay(), millis() etc. may not work as expected. Alternatives are made available.
* Support for the display has been removed to free up pins. Runs on Uno and Mega2560.

# Future work (piano-specific enhancements)
* Stereo effects across the length of the keyboard. The output circuit (described below) may be split so voices 0-7 go to the left channel, and voices 8-15 to the right.
* Pedal controls to modify enevelope and volume (soft peal, sostenuto, sustain)

# --------------------------------------------------------
# Arduino MIDI Sound Module
Turn your Arduino Uno into a 16-voice wavetable synthesizer/MIDI sound generator.

[![Image](media/Ardunio-MIDI-Sound-Module-Photo.jpg)](https://raw.githubusercontent.com/DLehenbauer/arduino-midi-sound-module/master/media/Ardunio-MIDI-Sound-Module-Photo.jpg)

# General MIDI Subset
Implements the most commonly used features of the General MIDI 1.0 standard, including:
* 128 standard instruments
* 45 percussion instruments
* 16 note polyphony with key velocity
* Note on/off, program change, pitch bend, channel volume, all notes and all sounds off.

# Demo

[![Arduino - Imperial March (Video)](media/Thumbnail.jpg)](https://www.youtube.com/watch?v=4pCgZSmhi4E)

# Synth Engine      
* 16 voices sampled & mixed in real-time at ~20kHz
* Wavetable and white noise sources
* Amplitude, frequency, and wavetable offset modulated by envelope generators
* Additional volume control per voice (used for channel volume and key velocity)
* Documented C++ code builds in Arduino IDE or Atmel Studio 6.0

# Hardware
[![Schematic](media/Ardunio-MIDI-Sound-Module-Plans.png)](https://raw.githubusercontent.com/DLehenbauer/arduino-midi-sound-module/master/media/Ardunio-MIDI-Sound-Module-Plans.png)

# Useful Tools
* [Hairless MIDI](https://projectgus.github.io/hairless-midiserial/) - Use default settings and `#define USE_HAIRLESS_MIDI`
* [The Kik Gen Labs - USBMidiKlik](https://github.com/TheKikGen/USBMidiKliK) - Do not `#define USE_HAIRLESS_MIDI`
* [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html)

# Resources
* [Open Music Labs - PWM DAC](http://www.openmusiclabs.com/learning/digital/pwm-dac.1.html)
* [Open Music Labs - Dual PWM Circuits](http://www.openmusiclabs.com/learning/digital/pwm-dac/dual-pwm-circuits/index.html)
