if ! [ -f ./libs.o ]; then
    wget -nc https://raw.githubusercontent.com/nothings/stb/master/stb_image.h https://raw.githubusercontent.com/nothings/stb/master/deprecated/stb_image_resize.h https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h

    echo '
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
    #define STB_IMAGE_RESIZE_IMPLEMENTATION
    #include "stb_image_resize.h"
    #define MINIAUDIO_IMPLEMENTATION
    #include "miniaudio.h"
    ' > libs.c

    cc -O3 -c libs.c -o libs.o
fi

cc -Wall -Wextra -O3 asciivideo.c -o asciivideo -lm libs.o