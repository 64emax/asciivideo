#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/time.h>

#include "stb_image.h"
#include "stb_image_resize.h"
#include "miniaudio.h"

const char *chrlist = "    ```...---'':::___,,,^^===;;;>>><<<++!!!rrrccc**///zzz???ssLLLTTTvvv)))JJ777(((|||FFiii{{{CCC}}}ffIII333111ttllluuu[[[nneeeoooZZZ555YYxxxjjjyyyaa]]]222EEESSwwwqqqkkkPPP66hhh999ddd44VVVpppOOOGGGbbUUUAAAKKKXXHHHmmm888RRDDD###$$$BBBgg000MMMNNNWWQQQ%%%&&&@@";

size_t frame = 1;
void incframe(int) {
	frame++;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "no video you stupid\n");
		exit(69);
	}

	bool nocolor = false;
	if (argc > 2) {
		if (strcmp(argv[2], "-nc") == 0 || strcmp(argv[2], "--no-color") == 0) {
			nocolor = true;
		}
	}

	char path[256];
	sprintf(path, "%s/audio.mp3", argv[1]);

	ma_engine engine;
	ma_engine_init(NULL, &engine);

	ma_engine_play_sound(&engine, path, NULL);

	size_t FPS = 60; //TODO: mayhaps don't hardcode
	size_t DT = (1.f/FPS)*1000000.f;

	struct itimerval h = { // ok posix
		.it_interval.tv_usec = DT,
		.it_value.tv_usec = DT,
	};
	setitimer(0, &h, NULL);
	signal(SIGALRM, incframe);

	size_t prevcol = 0;
	int imgw, imgh;
	struct winsize w;
	size_t OUTW, OUTH;

	while (true) {
		sprintf(path, "%s/%lu.png", argv[1], frame);
		if (access(path, F_OK) == -1) {
			ma_engine_uninit(&engine);
			if (frame == 1) {
				fprintf(stderr, "no video you stupid\n");
				return 69;
			}
			return 0;
		}

		ioctl(0, TIOCGWINSZ, &w);
		OUTW = w.ws_col;
		OUTH = w.ws_row;

		stbi_uc rimg[OUTW*OUTH*3];

		stbi_uc *img = stbi_load(path, &imgw, &imgh, NULL, 3);
		stbir_resize_uint8(img, imgw, imgh, 0, rimg, OUTW, OUTH, 0, 3);
		stbi_image_free(img);

		printf("\033[0;0H");
		for (size_t i = 0; i < OUTW*OUTH*3; i+=3) {
			if (nocolor || prevcol == (size_t)(rimg[i+2] << 16 | rimg[i+1] << 8 | rimg[i])) {
				printf("%c", chrlist[(rimg[i]+rimg[i+1]+rimg[i+2])/3]);
			}
			else {
				printf("\033[38;2;%d;%d;%dm%c", rimg[i], rimg[i+1], rimg[i+2], chrlist[(rimg[i]+rimg[i+1]+rimg[i+2])/3]);
			}
			prevcol = rimg[i+2] << 16 | rimg[i+1] << 8 | rimg[i];
		}
	}
}