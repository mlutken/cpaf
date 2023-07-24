#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <SDL2/SDL.h>

#include <cpaf_libs/audio/cpaf_audio_device.h>
#include <cpaf_libs/audio/cpaf_audio_system.h>

using namespace std;
using namespace std::chrono_literals;
using namespace cpaf;



#define SIMPLE_NO_MIXER 1
#define MIXER_PLAY_SAMPLES 2
#define CPAF_AUDIO_SYS_EX1 3

#define SDL_EXAMPLE CPAF_AUDIO_SYS_EX1

#if (SDL_EXAMPLE == CPAF_AUDIO_SYS_EX1)
static const std::string wave_file_path = "/home/ml/pCloudDrive/My Videos/torplay/songbird.wav";

// prototype for our audio callback
// see the implementation for more information
void my_audio_callback(uint8_t* stream, uint32_t length);

// variable declarations
static uint8_t* audio_pos; // global pointer to the audio buffer to be played
static uint32_t audio_len; // remaining length of the sample we have to play


/*
** PLAYING A SOUND IS MUCH MORE COMPLICATED THAN IT SHOULD BE
*/
int main(int /*argc*/, char* /*argv*/ []){
	std::cerr << "*** CPAF_AUDIO_SYS_EX1 ***\n";

	audio::system& sys = audio::system::instance();
	sys.init();
	std::cerr << sys.dbg_play_devices_str("Play devices") << "\n";
	std::cerr << "default_play_device_name: '" << sys.default_play_device_name() << "'\n";
//	audio::device dev(sys.default_play_device_name());
	audio::device dev;
	dev.play_callback_set(my_audio_callback);

	// local variables
	static uint32_t wav_length; // length of our sample
	static uint8_t *wav_buffer; // buffer containing our audio file
	static SDL_AudioSpec wav_spec; // the specs of our piece of music


	/* Load the WAV */
	// the specs, length and buffer of our wav are filled
	if( SDL_LoadWAV(wave_file_path.c_str(), &wav_spec, &wav_buffer, &wav_length) == NULL ){
		return 1;
	}
	audio_pos = wav_buffer; // copy sound buffer
	audio_len = wav_length; // copy file length

	/* Open the audio device */
	if (!dev.open()){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	std::cerr << "DEVICE NAME: '" << dev.device_name() << "'\n";
	dev.play();// Start playing


	// wait until done playing
	while ( audio_len > 0 ) {
		std::this_thread::sleep_for(100ms);
	}

	SDL_FreeWAV(wav_buffer);
}

// you should only copy as much as the requested length (len)
void my_audio_callback(uint8_t *stream, uint32_t length)
{
	if (audio_len ==0)
		return;

	length = ( length > audio_len ? audio_len : length );
	memcpy (stream, audio_pos, length); 					// simply copy from one buffer into the other

	audio_pos += length;
	audio_len -= length;
	std::cerr << "my_audio_callback: "
			  << " lenght: " << length
			  << " audio_len: " << audio_len
			  << " audio_pos: " << (void*)audio_pos
			  << "\n";
}
#elif (SDL_EXAMPLE == MIXER_PLAY_SAMPLES)
// --------------------------
// --- MIXER_PLAY_SAMPLES ---
// --------------------------
// https://soundprogramming.net/programming/tutorial-using-sdl2-and-sdl_mixer-to-play-samples/

constexpr int32_t NUM_WAVEFORMS = 4;
const char* _waveFileNames[] =
{
	"/home/ml/pCloudDrive/My Videos/torplay/Kick-Drum-1.wav",
	"/home/ml/pCloudDrive/My Videos/torplay/Snare-Drum-1.wav",
	"/home/ml/pCloudDrive/My Videos/torplay/Alesis-Fusion-Clean-Guitar-C3.wav",
	"/home/ml/pCloudDrive/My Videos/torplay/Alesis-Fusion-Tubular-Bells-C6.wav"
};

Mix_Chunk* _sample[NUM_WAVEFORMS];

// Initializes the application data
int Init(void)
{
	memset(_sample, 0, sizeof(Mix_Chunk*) * NUM_WAVEFORMS);

	// Set up the audio stream
	int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
	if( result < 0 )
	{
		fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	result = Mix_AllocateChannels(4);
	if( result < 0 )
	{
		fprintf(stderr, "Unable to allocate mixing channels: %s\n", SDL_GetError());
		exit(-1);
	}

	// Load waveforms
	for( int i = 0; i < NUM_WAVEFORMS; i++ )
	{
		_sample[i] = Mix_LoadWAV(_waveFileNames[i]);
		if( _sample[i] == NULL )
		{
			fprintf(stderr, "Unable to load wave file: %s\n", _waveFileNames[i]);
		}
	}

	return true;
}

int main(int /*argc*/, char** /*argv*/)
{
	// Initialize the SDL library with the Video subsystem
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO );
	atexit(SDL_Quit);

	/*SDL_Window* window =*/ SDL_CreateWindow("DrumPads",
											  SDL_WINDOWPOS_UNDEFINED,
											  SDL_WINDOWPOS_UNDEFINED,
											  256,
											  256,
											  SDL_WINDOW_RESIZABLE);

	// Application specific Initialize of data structures
	if (Init() == false)
		return -1;

	// Event descriptor
	SDL_Event Event;

	bool done = false;
	while (!done)
	{
		bool gotEvent = SDL_PollEvent(&Event);

		while (!done && gotEvent)
		{
			switch (Event.type)
			{
			case SDL_KEYDOWN:
				switch (Event.key.keysym.sym)
				{
				case 'q':
					Mix_PlayChannel(-1, _sample[0], 0);
					break;
				case 'w':
					Mix_PlayChannel(-1, _sample[1], 0);
					break;
				case 'e':
					Mix_PlayChannel(-1, _sample[2], 0);
					break;
				case 'a':
					Mix_PlayChannel(-1, _sample[3], 0);
					break;
				default:
					break;
				}
				break;

			case SDL_QUIT:
				done = true;
				break;

			default:
				break;
			}
			if( !done ) gotEvent = SDL_PollEvent(&Event);
		}
		std::this_thread::sleep_for(1ms);
	}

	for( int i = 0; i < NUM_WAVEFORMS; i++ )
	{
		Mix_FreeChunk(_sample[i]);
	}

	Mix_CloseAudio();
	SDL_Quit();
	return 0;
}

#elif (SDL_EXAMPLE == SIMPLE_NO_MIXER)
// -----------------------
// --- SIMPLE_NO_MIXER ---
// -----------------------

static const std::string wave_file_path = "/home/ml/pCloudDrive/My Videos/torplay/songbird.wav";

// prototype for our audio callback
// see the implementation for more information
void my_audio_callback(void *userdata, uint8_t *stream, int len);

// variable declarations
static uint8_t *audio_pos; // global pointer to the audio buffer to be played
static uint32_t audio_len; // remaining length of the sample we have to play


/*
** PLAYING A SOUND IS MUCH MORE COMPLICATED THAN IT SHOULD BE
*/
int main(int /*argc*/, char* /*argv*/ []){

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return 1;

	// local variables
	static uint32_t wav_length; // length of our sample
	static uint8_t *wav_buffer; // buffer containing our audio file
	static SDL_AudioSpec wav_spec; // the specs of our piece of music


	/* Load the WAV */
	// the specs, length and buffer of our wav are filled
	if( SDL_LoadWAV(wave_file_path.c_str(), &wav_spec, &wav_buffer, &wav_length) == NULL ){
		return 1;
	}
	// set the callback function
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = NULL;
	// set our global static variables
	audio_pos = wav_buffer; // copy sound buffer
	audio_len = wav_length; // copy file length

	/* Open the audio device */
	SDL_AudioSpec wav_spec_obtained; // the specs of our piece of music
	constexpr int iscapture = 0;
	constexpr int allowed_changes = 0;
	SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(nullptr, iscapture, &wav_spec, &wav_spec_obtained, allowed_changes);
//	SDL_AudioDeviceID device_id = SDL_OpenAudioDevice("JBL Charge 4", iscapture, &wav_spec, &wav_spec_obtained, allowed_changes);
	if ( device_id < 2 ){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudioDevice(device_id, 0); // Start playing

//	auto device_id = SDL_OpenAudio(&wav_spec, nullptr);
//	if ( device_id < 0 ){
//		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
//		exit(-1);
//	}
//	SDL_PauseAudio(0); // Start playing

	// wait until done playing
	while ( audio_len > 0 ) {
		SDL_Delay(100);
	}

	// shut everything down
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);

}

// audio callback function
// here you have to copy the data of your audio buffer into the
// requesting audio buffer (stream)
// you should only copy as much as the requested length (len)
void my_audio_callback(void* /*userdata*/, uint8_t *stream, int len) {

	uint32_t length = static_cast<uint32_t>(len);
	if (audio_len ==0)
		return;

	length = ( length > audio_len ? audio_len : length );
//	SDL_memcpy (stream, audio_pos, length); 					// simply copy from one buffer into the other
	memcpy (stream, audio_pos, length); 					// simply copy from one buffer into the other
	//	SDL_MixAudio(stream, audio_pos, length, SDL_MIX_MAXVOLUME);// mix from one buffer into another

	audio_pos += length;
	audio_len -= length;
	std::cerr << "my_audio_callback, len: " << len
			  << " lenght: " << length
			  << " audio_len: " << audio_len
			  << " audio_pos: " << (void*)audio_pos
			  << "\n";
}
#endif
