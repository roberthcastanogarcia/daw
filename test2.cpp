#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int AMPLITUDE = 28000;
const int SAMPLE_RATE = 44100;

// Mi tipo de prueba, el cuál vamos a usar para enviar el audio y su longitud
typedef struct
{
	Uint32 length;
	Uint8 *buffer;
} sound;

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes)
{
	
	Uint8 *buffer = (Uint8*)raw_buffer;
	int length = bytes;

	//Convertimos el puntero user_data en tipo sound
	sound &s(*(sound*)user_data);

	for(int i = 0; i < length; i++)
	{
		buffer[i] = s.buffer[i] * AMPLITUDE;
	}
	
}

// La forma más básica de encapsulación en c++ es una función

// Función que devuelve un búfer de audio relleno con datos aleatorios



int main(int argc, char *argv[])
{
	// Inicialización de SDL_Audio con prueba
	if(SDL_Init(SDL_INIT_AUDIO) != 0) printf("Fallo al inicializar SDL: %s", SDL_GetError());
	
	//----- Aquí se genera el sonido -----
	
	int sample_nr = 0;

	// Declaro el sonido y lo lleno con valores aleatorios
	sound s;
	s.length = 32;
	s.buffer = (Uint8*) malloc(sizeof(Uint8) * s.length);
	
	srand(time(NULL));
	
	for (int i = 0; i < s.length; i++){
		s.buffer[i] = rand() + 1;
	}
	
	//----- Aquí se termina de generar -----

	SDL_AudioSpec want;
	want.freq = SAMPLE_RATE;
	want.format = AUDIO_S16SYS; // signed short 16 bit
	want.channels = 2; // mono
	want.samples = 1024; // Tamaño del buffer
	want.callback = audio_callback; // Función que se llama periódicamente para rellenar el buffer
	//want.userdata = &sample_nr;
	want.userdata = &s;

	SDL_AudioSpec have;
	if(SDL_OpenAudio(&want, &have) != 0) printf("Failed to open audio: %s", SDL_GetError());
	if(want.format != have.format) printf("Failed to get the desired AudioSpec", SDL_GetError());

	SDL_PauseAudio(0); //Start playing sound
	SDL_Delay(3000); //wait while sound is playing
	SDL_PauseAudio(1); //Stop plying sound

	SDL_CloseAudio();

	// No se debe olvidar de limpiar el buffer
	free(s.buffer);
	return 0;
}
