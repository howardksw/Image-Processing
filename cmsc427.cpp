#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Image.hpp"

// Program arguments
static char options[] =
"\n"
"  -help\n"
"  -bilateral_filter <real:domain> <real:range>\n"
"  -blackandwhite \n"
"  -brightness <real:factor>\n"
"  -channel_extract <int:channel (0=red,1=green,2=blue,3=alpha)>\n"
"  -composite <file:bottom_mask> <file:top_image> <file:top_mask> <int:operation(0=over)>\n"
"  -contrast <real:factor>\n"
"  -crop <int:x> <int:y> <int:width> <int:height>\n"
"  -fun\n"
"  -gamma <real:exponent>\n"
"  -gaussian_blur <real:sigma>\n"
"  -median_filter <int:width>\n"
"  -motion_blur <real:magnitude>\n"
"  -nonphotorealism\n"
"  -rotate <real:angle (in degrees)> \n"
"  -sampling <int:method (0=point [default],1=bilinear,2=gaussian)>\n"
"  -saturation <real:factor>\n"
"  -scale <real:sx> <real:sy>\n"
"  -sharpen\n";


// Print usage message and exit
static void ShowUsage(void)
{
    fprintf(stderr, "Usage: cmsc427 <jpg:input_image> <jpg:output_image> [  -option [arg ...] ...]\n");
    fprintf(stderr, "%s", options);
    exit(EXIT_FAILURE);
}


// Check if there are enough remaining arguments for option
static void CheckOption(char *option, int argc, int minargc)
{
    if (argc < minargc)  {
        fprintf(stderr, "Too few arguments for %s\n", option);
        ShowUsage();
        exit(-1);
    }
}


// Application start point
int main(int argc, char *argv[])
{
    // Look for help
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-help")) {
            ShowUsage();
        }
    }

    // Set the default sampling method to use
    int sampling_method = IMAGE_POINT_SAMPLING;
    // See if a different sampling method was specified
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-sampling")) {
            int method = atoi(argv[i+1]);
            if (method == 0) {
                sampling_method = IMAGE_POINT_SAMPLING;
            }
            else if (method == 1) {
                sampling_method = IMAGE_BILINEAR_SAMPLING;
            }
            else if (method == 2) {
                sampling_method = IMAGE_GAUSSIAN_SAMPLING;
            }
            else {
                fprintf(stderr, "Sampling method specified incorrectly.\n");
                ShowUsage();
                exit(-1);
            }
        }
    }

    // Read input and output image filenames
    if (argc < 3) ShowUsage();
    argv++, argc--; // First argument is program name
    char *input_image_name = *argv; argv++, argc--;
    char *output_image_name = *argv; argv++, argc--;

    // Allocate memory for image
    Image *image = new Image();
    if (!image) {
        fprintf(stderr, "Unable to allocate image\n");
        exit(-1);
    }

    // Read input image
    if (!image->Read(input_image_name)) {
        fprintf(stderr, "Unable to read image from %s\n", input_image_name);
        exit(-1);
    }

    // Parse arguments in order (left to right) and perform operations
    while (argc > 0) {
        if (!strcmp(*argv, "-bilateral_filter")) {
            CheckOption(*argv, argc, 3);
            double sx = atof(argv[1]);
            double sy = atof(argv[2]);
            argv += 3; argc -= 3;
            image->BilateralFilter(sy, sx);
        }
        else if (!strcmp(*argv, "-blackandwhite")) {
            argv++, argc--;
            image->BlackAndWhite();
        }
        else if (!strcmp(*argv, "-brightness")) {
            CheckOption(*argv, argc, 2);
            double factor = atof(argv[1]);
            argv += 2; argc -=2;
            image->Brightness(factor);
        }
        else if (!strcmp(*argv, "-channel_extract")) {
            CheckOption(*argv, argc, 2);
            int channel = atoi(argv[1]);
            argv += 2; argc -= 2;
            image->ChannelExtract(channel);
        }
        else if (!strcmp(*argv, "-composite")) {
            CheckOption(*argv, argc, 5);
            argv += 5; argc -= 5;
            image->Composite();
        }
        else if (!strcmp(*argv, "-contrast")) {
            CheckOption(*argv, argc, 2);
            double factor = atof(argv[1]);
            argv += 2; argc -= 2;
            image->Contrast(factor);
        }
        else if (!strcmp(*argv, "-crop")) {
            CheckOption(*argv, argc, 5);
            int x = atoi(argv[1]);
            int y = atoi(argv[2]);
            int w = atoi(argv[3]);
            int h = atoi(argv[4]);
            argv += 5; argc -= 5; // remove the arguments from the list
            image->Crop(x, y, w, h);
        }
        else if (!strcmp(*argv, "-fun")) {
            argv++, argc--;
            image->Fun(sampling_method);
        }
        else if (!strcmp(*argv, "-gamma")) {
            CheckOption(*argv, argc, 2);
            double factor = atof(argv[1]);
            argv += 2; argc -= 2;
            image->Gamma(factor);
        }
        else if (!strcmp(*argv, "-gaussian_blur")) {
            CheckOption(*argv, argc, 2);
            double sigma = atof(argv[1]);
            argv += 2; argc -= 2;
            image->GaussianBlur(sigma);
        }
        else if (!strcmp(*argv, "-median_filter")) {
            CheckOption(*argv, argc, 2);
            int width = atoi(argv[1]);
            argv += 2; argc -= 2;
            image->MedianFilter(width);
        }
        else if (!strcmp(*argv, "-motion_blur")) {
            CheckOption(*argv, argc, 2);
            double sigma = atof(argv[1]);
            argv += 2; argc -= 2;
            image->MotionBlur(sigma);
        }
        else if (!strcmp(*argv, "-nonphotorealism")) {
            argv++, argc--;
            image->Nonphotorealism();
        }
        else if (!strcmp(*argv, "-rotate")) {
            CheckOption(*argv, argc, 2);
            double angle = atof(argv[1]);
            argv += 2; argc -= 2;
            image->Rotate(angle, sampling_method);
        }
        else if (!strcmp(*argv, "-sampling")) {
            // skip this flag. it has already been set above.
            argv += 2; argc -= 2;
        }
        else if (!strcmp(*argv, "-saturation")) {
            CheckOption(*argv, argc, 2);
            double factor = atof(argv[1]);
            argv += 2; argc -= 2;
            image->Saturation(factor);
        }
        else if (!strcmp(*argv, "-scale")) {
            CheckOption(*argv, argc, 3);
            double sx = atof(argv[1]);
            double sy = atof(argv[2]);
            argv += 3; argc -= 3;
            image->Scale(sx, sy, sampling_method);
        }
        else if (!strcmp(*argv, "-sharpen")) {
            argv++, argc--;
            image->Sharpen();
        }
        else {
            // Unrecognized program argument
            fprintf(stderr, "image: invalid option: %s\n", *argv);
            ShowUsage();
        }
    }

    // Write output image
    if (!image->Write(output_image_name)) {
        fprintf(stderr, "Unable to write image to %s\n", output_image_name);
        exit(EXIT_FAILURE);
    }

    // Delete image (we don't want a memory leak!)
    delete image;

    // exit and return success
    exit(EXIT_SUCCESS);
}
