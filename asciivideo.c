#include "finnish.h"

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>

#include "miniaudio.h"
#include "stb_image.h"
#include "stb_image_resize.h"

vakio merkki *merkkilista =
    "    "
    "```...---'':::___,,,^^===;;;>>><<<++!!!rrrccc**///"
    "zzz???ssLLLTTTvvv)))JJ777(((|||FFiii{{{CCC}}}"
    "ffIII333111ttllluuu[[[nneeeoooZZZ555YYxxxjjjyyyaa]]]"
    "222EEESSwwwqqqkkkPPP66hhh999ddd44VVVpppOOOGGGbbUUUAAAKKK"
    "XXHHHmmm888RRDDD###$$$BBBgg000MMMNNNWWQQQ%%%&&&@@";

koko_t frame = 1;
tyhjyys incframe(kokonaisluku) { frame++; }

kokonaisluku main(kokonaisluku argc, merkki **argv) {
  jos(argc < 2) {
    fprintf(stderr, "ei videota idiootti\n");
    exit(69);
  }

  totuusarvo eiväriä = epätosi;
  jos(argc > 2) {
    jos(strcmp(argv[2], "-nc") == 0 || strcmp(argv[2], "--no-color") == 0) {
      eiväriä = tosi;
    }
  }

  merkki tie[256];
  sprintf(tie, "%s/audio.mp3", argv[1]);

  ma_engine moottori;
  ma_engine_init(TYHJÄ, &moottori);

  ma_engine_play_sound(&moottori, tie, TYHJÄ);

  koko_t FPS = 60; // TODO: ehkä älä oleta
  koko_t DT = (1.f / FPS) * 1000000.f;

  rakenne itimerval h = {
      // ok posix
      .it_interval.tv_usec = DT,
      .it_value.tv_usec = DT,
  };
  setitimer(0, &h, TYHJÄ);
  signal(SIGALRM, incframe);

  koko_t edellinen_sarake = 0;
  kokonaisluku kuval, kuvak;
  rakenne winsize w;
  koko_t OUTW, OUTH;

  kun(tosi) {
    sprintf(tie, "%s/%lu.png", argv[1], frame);
    jos(access(tie, F_OK) == -1) {
      ma_engine_uninit(&moottori);
      jos(frame == 1) {
        fprintf(stderr, "ei videota idiootti\n");
        palauta 69;
      }
      palauta 0;
    }

    ioctl(0, TIOCGWINSZ, &w);
    OUTW = w.ws_col;
    OUTH = w.ws_row;

    stbi_uc okuva[OUTW * OUTH * 3];

    stbi_uc *kuva = stbi_load(tie, &kuval, &kuvak, TYHJÄ, 3);
    stbir_resize_uint8(kuva, kuval, kuvak, 0, okuva, OUTW, OUTH, 0, 3);
    stbi_image_free(kuva);

    printf("\033[0;0H");
    varten(koko_t i = 0; i < OUTW * OUTH * 3; i += 3) {
      jos(eiväriä ||
          edellinen_sarake ==
              (koko_t)(okuva[i + 2] << 16 | okuva[i + 1] << 8 | okuva[i])) {
        printf("%c", merkkilista[(okuva[i] + okuva[i + 1] + okuva[i + 2]) / 3]);
      }
      muuten {
        printf("\033[38;2;%d;%d;%dm%c", okuva[i], okuva[i + 1], okuva[i + 2],
               merkkilista[(okuva[i] + okuva[i + 1] + okuva[i + 2]) / 3]);
      }
      edellinen_sarake = okuva[i + 2] << 16 | okuva[i + 1] << 8 | okuva[i];
    }
  }
}
