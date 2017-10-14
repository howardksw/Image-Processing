#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string.h>
#include <QtGui>

#define IMAGE_RETURN_SUCCESS true
#define IMAGE_RETURN_FAILURE false

typedef enum {
    IMAGE_POINT_SAMPLING,
    IMAGE_BILINEAR_SAMPLING,
    IMAGE_GAUSSIAN_SAMPLING
} ImageSamplingMethod;


typedef enum {
    IMAGE_RED_CHANNEL,
    IMAGE_GREEN_CHANNEL,
    IMAGE_BLUE_CHANNEL,
    IMAGE_ALPHA_CHANNEL,
    IMAGE_NUM_CHANNELS
} ImageChannel;

using namespace std;
class Image {
public:
    /*
    class constructor
    */
    Image();
    Image(const char *filename);

    /*
    class destructor
    */
    ~Image();

    /*
    Reads a HDR image into an array called image_data
    */
    bool Read(const char *filename);

    /*
    Writes a new HDR image based on the contents of image_data
    */
    bool Write(const char *filename);

    /*
    A description of your implementation for this method goes here
    */
    void BilateralFilter(double rangesigma, double domainsigma);

    /*
    A description of your implementation for this method goes here
    */
    void BlackAndWhite();

    /*
    A description of your implementation for this method goes here
    */
    void Brightness(double factor);

    /*
    Extract the specified channel from the Image (i.e. remove all other channels)
    0=red,1=green,2=blue,3=alpha
    */
    void ChannelExtract(int channel);

    /*
    A description of your implementation for this method goes here
    */
    void Composite();

    /*
    A description of your implementation for this method goes here
    */
    void Contrast(double factor);

    /*
    Performs a crop of the image with the following parameters
    top_left_x: the x coordinate of the top left point of the crop window
    top_left_y: the y coordinate of the top left point of the crop window
    crop_width = width of the crop window to be cut out
    crop_height = height of the crop window to be cut out
    NOTE: all 4 corners of the crop window can be determined by these parameters
    */
    void Crop(int top_left_x, int top_left_y, int crop_width, int crop_height);

    /*
    A description of your implementation for this method goes here
    */
    void Fun(int sampling_method);

    /*
    A description of your implementation for this method goes here
    */
    void Gamma(double factor);

    /*
    Performs a gaussian blur in both x and y direction with a specified sigma
    */
    void GaussianBlur(double sigma);

    /*
    A description of your implementation for this method goes here
    */
    void MedianFilter(int filter_width);

    /*
    A description of your implementation for this method goes here
    */
    void MotionBlur(double sigma);

    /*
    A description of your implementation for this method goes here
    */
    void Nonphotorealism();

    /*
    A description of your implementation for this method goes here
    */
    void Rotate(double angle, int sampling_method);

    /*
    A description of your implementation for this method goes here
    */
    void Saturation(double factor);

    /*
    Scales an image down/up in the x and y direction using a given scale factor and interpolation method.
    NOTE: a scale factor of 1 will not change the image. For example, to scale
    the image up by a factor of 2, scale factor = 2. To scale an image down
    by half, scale factor = 0.5
    */
    void Scale(double sx, double sy, int sampling_method);

    /*
    A description of your implementation for this method goes here
    */
    void Sharpen();

private:
    QImage image_data;
    int width;
    int height;
    int npixels;
};

#endif
