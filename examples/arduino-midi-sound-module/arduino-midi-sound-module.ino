/*
    Arduino Midi Sound Library

    Forked from Arduino Midi Synth v0.2.2
    https://github.com/DLehenbauer/arduino-midi-sound-module
    Midi communication and display removed, some piano-specific functionality
    added, and made into a library. See readme.md for details.

    For more sound generation, see:
      dac.h          - 2-channel PWM DAC
      synth.h        - Core synth engine
      envelope.h     - Flexible envelope generator used to modulate amp, freq, and wave
      midisynth.h    - Extends synth engine w/additional state for MIDI processing
    instruments*.h - Wavetable and envelope generator programs for GM MIDI instruments


    This sketch turns an Arduino Uno into a 16-voice wavetable synthesizer functioning as a
    MIDI sound module.  Typical usages include generating sound for a MIDI keyboard controller
    or playback of MIDI files.

    The MIDI synth implements the most commonly used features of the General MIDI 1.0 standard,
    including:

      - 128 standard instruments
      - 45 percussion instruments
      - Key velocity
      - Pitch bend

    (See open GitHub issues for caveats: https://github.com/dlehenbauer/arduino-midi-sound-module/issues)

    The synth engine features:
      - 16 voices sampled & mixed in real-time at ~20kHz
      - Wavetable and white noise sources
      - Amplitude, frequency, and wavetable offset modulated by envelope generators
      - Additional volume control per voice (matching MIDI velocity)

    The circuit for one channel. For two channels, split pins 5/6 from 9/10 and give each
    pair its own output coupling and filtering capacitor.
    For this variation, #define STEREO_PIANO in midiNoteOn (midisynth.h)

    Connection to Arduino Uno:(pin numbers differ on Mega)

        Uno     Mega
                              1M (1%)                  10uf*
        pin 5   pin 4   >----^v^v^------o--------o------|(----> audio out
                                        |        |
                             3.9k (1%)  |       === 3.3nf**
        pin 6   pin 13  >----^v^v^------o        |
                                        |       gnd
                                        |
                                        |
                              1M (1%)   |
        pin 9   pin 11  >----^v^v^------o
                                        |
                             3.9k (1%)  |
       pin 10   pin 12  >----^v^v^------'


      * Note: A/C coupling capacitor typically optional.  (Negative is on audio out side.)
     ** Note: RC filtering capacitor is optional and can be adjusted to taste:

                          8kHz      10kHz      30kHz
               2.2nf ~=  -0.7db    -1.1db     -5.6db
               3.3nf ~=  -1.5db    -2.2db     -8.4db
               4.7nf ~=  -2.7db    -3.6db    -11.1db

    Note that all these values still result in a lot of 67kHz through each side
    of the stereo version of this circuit (since there is no cancellation).
    Use a larger cap, like 22nF, to get these down to an acceptable level.

*/

#include <stdint.h>
// TODO Adjust this when the library is bedded down.
#include "C:\Users\giles\Dropbox\Arduino\arduino-midi-sound-library\src\midisynth.h"

MidiSynth synth;
static int inst = 0, curr_inst = 0;
static bool new_inst = false;

// Invoked once after the device is reset, prior to starting the main 'loop()' below.
void setup() {
  // Serial port for debugging.
  Serial.begin(9600);
  while (!Serial)
    ;

  synth.begin();                          // Start synth sample/mixing on Timer2 ISR

  sei();                                  // Begin processing interrupts.
}

void loop() {
  // Octave starting at middle C, white notes only. A440 is note 69.
  int names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
  int octave[] = {60,  62,  64,  65,  67,  69,  71,  72};
  static uint32_t off_time[] = {0,  0,   0,   0,   0,   0,   0,   0};

  // Pick up some simple (single-key) keyboard commands from the serial monitor.
  // Play one or more notes in the middle-C octave, optionally prefixed by an
  // instrument change.
  if (Serial.available())
  {
    int key = Serial.read();

    for (int i = 0; i < 8; i++)
    {
      if (key == names[i])
      {
        // If we've changed instrument, set it up.
        if (new_inst && inst != curr_inst)
        {
          char name[80];

          Instruments::getInstrumentName(inst, name);
          Serial.print(inst);
          Serial.print(": ");
          Serial.println(name);
          synth.midiProgramChange(1, inst);
          curr_inst = inst;
          inst = 0;     // ready for next instrument change
          new_inst = false;
        }

        // Sound the note.
        synth.midiNoteOn(1, octave[i], 127);
        off_time[i] = synth.getDelayCount();
        break;
      }

    }

    // Change instrument to the numbered.
    // Accept a 2 or 3 digit number here by building up digits.
    if (key >= '0' && key <= '9')
    {
      inst = (inst * 10) + (key - '0');
      new_inst = true;
    }
  }

// Define this to stop notes after a delay. Note that organs, etc. can only
// be stopped this way.
//#define STOP_NOTES
#ifdef STOP_NOTES
  for (int i = 0; i < 8; i++)
  {
    // Process any noteOff calls. noteOff after 0.5 second delay.
    if (off_time[i] != 0 && synth.getDelayCount() - off_time[i] > 10000)  // 0.5 sec delay (20 ticks = 1ms)
    {
      synth.midiNoteOff(1, octave[i]);
      off_time[i] = 0;
    }
  }
#endif // STOP_NOTES
}
