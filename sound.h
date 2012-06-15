#ifndef SOUND_H
#define SOUND_H

#define TEMPO 120.0 //bpm
#define NOTE_C 261.625
#define NOTE_Db 277.182
#define NOTE_D 293.664
#define NOTE_Eb 311.126
#define NOTE_E 329.627
#define NOTE_F 349.228
#define NOTE_Gb 369.994
#define NOTE_G 391.995
#define NOTE_Ab 415.304
#define NOTE_A 440.000
#define NOTE_Bb 466.163	
#define NOTE_B 493.883
#define NOTE_Co 523.251

void playmusic(char* s);
void beep(float frequency, int ms);

#endif
