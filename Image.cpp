#include "Image.hpp"

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream> // keep this if you prefer to use cout over printf

using namespace std;

Image::Image()
: npixels(0), width(0), height(0)
{}

Image::Image(const char *filename)
    : npixels(0), width(0), height(0)
{
    if (!Read(filename)){
        printf("Image not created");
    }
}

Image::~Image() {
    //delete image_data;
}

bool Image::Read(const char *filename)
{
    // load image file
    QImage image(QString(filename), "JPG");
    if (image.isNull()) {
        return IMAGE_RETURN_FAILURE;
    }
    // convert to 32-bit image where each pixel is a QRgb
    image_data = image.convertToFormat(QImage::Format_RGB32);
    if (image_data.isNull()) {
        return IMAGE_RETURN_FAILURE;
    }

    width = image_data.width();
    height = image_data.height();
    npixels = width * height;

    QImage new_data(500,1000, QImage::Format_RGB32);
    QColor color(50,50,50);
    new_data.setPixel(50, 87, color.rgb());

    return IMAGE_RETURN_SUCCESS;
}


bool Image::Write(const char *filename)
{
    if (image_data.save(QString(filename), "JPG")) {
        return IMAGE_RETURN_SUCCESS;
    } else {
        return IMAGE_RETURN_FAILURE;
    }
}


void Image::BilateralFilter(double rangesigma, double domainsigma)
{
    printf("Must implement BilateralFilter()\n");
}


void Image::BlackAndWhite()
{
    int r, g, b;
    // iterate through the image
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            // get pixel
            QColor new_image(image_data.pixel(i,j));
            // take the average of the RGB value to get black and white
            int avg = (new_image.red() + new_image.green() + new_image.blue())/3;
            avg = qBound(0,avg,255);
            image_data.setPixel(i,j,qRgb(avg,avg,avg));
        }
    }
}


void Image::Brightness(double factor)
{
    int r, g, b;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            QColor new_image(image_data.pixel(i,j));
            // After getting the RGB values from each pixel, multiply each value with factor
            r = new_image.red() * factor;
            g = new_image.green() * factor;
            b = new_image.blue() * factor;

            // clamp
            r = qBound(0,r,255);
            g = qBound(0,g,255);
            b = qBound(0,b,255);
            image_data.setPixel(i,j,qRgb(r,g,b));
        }
    }
}


// 0=red,1=green,2=blue,3=alpha
void Image::ChannelExtract(int channel)
{
    if(channel == 0){
        for(int i= 0; i< width;i++){
             for(int j= 0; j<height;j++){
                 QColor original(image_data.pixel(i,j));
                 int r = original.red();
                 // set red only
                 image_data.setPixel(i,j,qRgb(r,0,0));
             }
        }
    }else if(channel == 1){
        for(int i= 0; i< width;i++){
            for(int j= 0; j<height;j++){
                QColor original(image_data.pixel(i,j));
                int g = original.green();
                // set green only
                image_data.setPixel(i,j,qRgb(0,g,0));
            }
        }
    }else if(channel == 2){
        for(int i= 0; i< width;i++){
            for(int j= 0; j<height;j++){
                QColor original(image_data.pixel(i,j));
                int b = original.blue();
                // set blue only
                image_data.setPixel(i,j,qRgb(0,0,b));
            }
        }
    }

    if(channel == 3)
    {
        for(int i= 0; i< width;i++){
            for(int j= 0; j<height;j++){
                QColor original(image_data.pixel(i,j));
                int a = original.alpha();
                // set alpha only
                image_data.setPixel(i,j,qRgba(0,0,0,a));
            }
        }
    }
}


void Image::Composite()
{
    printf("Must implement Composite()\n");
}


void Image::Contrast(double factor)
{
    // contrast: new color = 128 + Contrast * (old_color -128)
    int r, g, b;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            QColor new_image(image_data.pixel(i,j));
            r = ((new_image.red() - 128) * factor) + 128;
            g = ((new_image.green() - 128) * factor) + 128;
            b = ((new_image.blue() - 128) * factor) + 128;

            // clamp
            r = qBound(0,r,255);
            g = qBound(0,g,255);
            b = qBound(0,b,255);
            image_data.setPixel(i,j,qRgb(r,g,b));
        }
    }
}


void Image::Crop(int top_left_x, int top_left_y, int crop_width, int crop_height)
{
    QColor color;
    QImage image_new(crop_width,crop_height,QImage::Format_RGB32);

    // iterate through the image with the desired range
    for(int x = top_left_x; x < crop_width + top_left_x; x++){
        for(int y = top_left_y; y < crop_height + top_left_y; y++){

            color = QColor(image_data.pixel(x,y));
            // set the RGB values from the old image to new image
            image_new.setPixel(x-top_left_x,y-top_left_y,qRgb(color.red(),color.green(),color.blue()));
        }
    }
    image_data = image_new;
}


void Image::Fun(int sampling_method)
{
    printf("Must implement Fun()\n");
}


void Image::Gamma(double factor)
{
    printf("Must implement Gamma()\n");
}


void Image::GaussianBlur(double sigma)
{
    printf("Must implement GaussianBlur()\n");
}


void Image::MedianFilter(int filter_width)
{
        // initialize RGB and intensity arrays
        int r_arr[filter_width*filter_width];
        int g_arr[filter_width*filter_width];
        int b_arr[filter_width*filter_width];
        int i_arr[filter_width*filter_width];
        int index = 0;
        QColor color;
        QImage image_new(width,height,QImage::Format_RGB32);

        int gap = filter_width/2;
        for(int x = gap; x < width - gap; x++){
              for(int y = gap; y < height - gap; y++){
                  index = -1;
                  for(int i = -gap; i <= gap; i++){
                      for(int j = -gap; j <= gap; j++){
                          color = QColor(image_data.pixel(x+j,y+i));
                          index += 1;
                          // store the values into the arrays
                          r_arr[index] = color.red();
                          g_arr[index] = color.green();
                          b_arr[index] = color.blue();
                          i_arr[index] = (color.red()+color.green()+color.blue())/3;
                      }
                  }

                  // find median of each array
                  int median_pos = (filter_width*filter_width/2)+1;
                  int a, b, flag = 1;    // set flag to 1 to start first pass
                  int temp,temp1,temp2,temp3;             // holding variables
                  int numLength = filter_width*filter_width;

                  // bubble sorting
                  for(a = 1; (a <= numLength) && flag; a++)
                  {
                      flag = 0;
                      for (b=0; b < (numLength -1); b++)
                     {
                           if (i_arr[b+1] > i_arr[b])
                          {
                                temp = i_arr[b];
                                temp1 = r_arr[b];
                                temp2 = g_arr[b];
                                temp3 = b_arr[b];
                                i_arr[b] = i_arr[b+1];
                                r_arr[b] = r_arr[b+1];
                                g_arr[b] = g_arr[b+1];
                                b_arr[b] = b_arr[b+1];
                                i_arr[b+1] = temp;
                                r_arr[b+1] = temp1;
                                g_arr[b+1] = temp2;
                                b_arr[b+1] = temp3;
                                flag = 1;
                           }
                      }
                 }
                  int i_med = i_arr[median_pos];
                  int r_med = r_arr[median_pos];
                  int g_med = g_arr[median_pos];
                  int b_med = b_arr[median_pos];

                  image_new.setPixel(x,y, qRgb(r_med,g_med,b_med));
        }
    }
    image_data = image_new;
}


void Image::MotionBlur(double sigma)
{
        printf("Must implement Sharpen()\n");
}


void Image::Nonphotorealism()
{
    printf("Must implement Nonphotorealism()\n");
}


void Image::Rotate(double angle, int sampling_method)
{

    double pi = 3.1415926;
    double rad = (angle/360)*2*pi;
    int new_width = abs(height * sin(rad)) + abs(width *cos(rad));
    int new_height = abs(width * sin(rad)) + abs(height * cos(rad));

    QImage new_img = QImage(new_width,new_height,QImage::Format_RGB32);

    if(sampling_method == IMAGE_POINT_SAMPLING){

        // resverse mapping
        for(int w = 0; w < new_width; w++){
              for(int h = 0; h < new_height; h++){
                  // apply the transformation so that the center point is the origin
                  int trans_w = w - (new_width-1)/2;
                  int trans_h = h - (new_height-1)/2;
                  // calculate the corresponding pixel
                  int trans_w0 = trunc((trans_w * cos(-rad) - trans_h * sin(-rad)) + 0.5);
                  int trans_h0 = trunc((trans_w * sin(-rad) + trans_h * cos(-rad)) + 0.5);
                  // transform the point back to top-left corner as origin
                  int w0 = trans_w0 + (width-1)/2;
                  int h0 = trans_h0 + (height-1)/2;
                  // check if the old pixel is in the range
                  if(w0 < width && w0 >= 0 && h0 >=0 && h0 < height){
                      // get data of old pixel
                      QColor color = QColor(image_data.pixel(w0, h0));

                      //check is w and h are in bound
                      new_img.setPixel(w, h, qRgb(color.red(),color.green(),color.blue()));
                  }
              }
        }
    }else if (sampling_method == IMAGE_BILINEAR_SAMPLING) {
        for(int w = 0; w <= (new_width - 1); w++){
            for(int h = 0; h <= (new_height - 1); h++){
            // transform the center to the origin
                int trans_w = w - (new_width-1)/2;
                int trans_h = h - (new_height-1)/2;
                // calculate pixel
                double trans_w0 = trans_w * cos(-rad) - trans_h * sin(-rad);
                double trans_h0 = trans_w * sin(-rad) + trans_h * cos(-rad);

                // transform the point back to top-left corner as origin
                double w0 = trans_w0 + (width-1)/2;
                double h0 = trans_h0 + (height-1)/2;
                // get data of old pixel
                if(w0 <=(width -1) && w0 >= 0 && h0 <= (height-1) && h0 >=0){
                    int x1 = floor(w0);
                    int x2 = x1+1;
                    int y1 = floor(h0);
                    int y2 = y1+1;
                    double dx = w0-x1;
                    double dy = h0-y1;

                    QColor colorx1y1 = QColor(image_data.pixel(x1, y1));
                    QColor colorx2y1 = QColor(image_data.pixel(x2, y1));
                    QColor colorx1y2 = QColor(image_data.pixel(x1, y2));
                    QColor colorx2y2 = QColor(image_data.pixel(x2, y2));

                    double aR = colorx1y1.red()*(1-dx) + colorx2y1.red()*dx;
                    double bR = colorx1y2.red()*(1-dx) + colorx2y2.red()*dx;
                    int destR = aR*(1-dy) +bR*dy;
                    double aG = colorx1y1.green()*(1-dx) + colorx2y1.green()*dx;
                    double bG = colorx1y2.green()*(1-dx) + colorx2y2.green()*dx;
                    int destG = aG*(1-dy) +bG*dy;
                    double aB = colorx1y1.blue()*(1-dx) + colorx2y1.blue()*dx;
                    double bB = colorx1y2.blue()*(1-dx) + colorx2y2.blue()*dx;
                    int destB = aB*(1-dy) +bB*dy;

                    //check is w and h are in bound
                    new_img.setPixel(w, h, qRgb(destR,destG,destB));
                 }
                 else{
                    new_img.setPixel(w, h, qRgb(0,0,0));
                 }
            }
        }
    }else if(sampling_method == IMAGE_GAUSSIAN_SAMPLING) {
        /*iterate over new image pixels  */
        for(int x = 0; x<new_width-1; x++){
              for(int y = 0;y<new_height-1; y++){
                  // calculate old pixel position
                  int trans_x = x - (new_width-1)/2;
                  int trans_y = y - (new_height-1)/2;
                  // calculate the pixel
                  double trans_x0 = trans_x * cos(-rad) - trans_y * sin(-rad);
                  double trans_y0 = trans_x * sin(-rad) + trans_y * cos(-rad);

                  // transform back to the old image
                  double x0 = trans_x0 + (width-1)/2;
                  double y0 = trans_y0 + (height-1)/2;
                  double sgm = 2;

                  if(x0 <(width -1) && x0 >= 0 && y0 < (height-1) && y0 >=0){

                    //resample using Gausian
                    double resample_val_r = 0, norm_const= 0;
                    double resample_val_g = 0;
                    double resample_val_b = 0;
                    int r,g,b;
                    for(int i = 0; i < width; i++) {
                        for(int j = 0; j < height; j++) {
                            double dist = sqrt(pow(x0-i,2)+pow(y0-j,2));
                            if (dist <= sgm) {
                                // see if we need the original point or not

                                (r,g,b) = image_data.pixel(i, j);
                                //norm const assures the weights all add up to 1
                                int gaus = exp(-0.5*pow(x0-i,2)/pow(sgm,2)-0.5*pow(y0-j,2)/pow(sgm,2));
                                norm_const += gaus;

                                //gausian function evaluated
                                (resample_val_r,resample_val_g,resample_val_b) += gaus*(r,g,b) ;
                            }
                        }
                    }

                    //normalized
                    (r,g,b)= (resample_val_r,resample_val_g,resample_val_b) / norm_const;

                    new_img.setPixel(x,y, qRgb(r,g,b));
                }
            }
        }
    }
    image_data = new_img;
}


void Image::Saturation(double factor)
{
    printf("Must implement Saturation()\n");
}


void Image::Scale(double sx, double sy, int sampling_method)
{
    int new_width = sx * width;
    int new_height = sy * height;

    QImage new_img = QImage(new_width,new_height,QImage::Format_RGB32);
    int x0,y0;

    if(sampling_method == IMAGE_POINT_SAMPLING){
        // point resampling
        for(int x = 0; x<=new_width-1; x++){
            for(int y = 0; y<=new_height-1; y++){
                // copy over
                x0 = trunc((x/sx)+0.5);
                y0 = trunc((y/sy)+0.5);
                // get data of old pixel

                if(x0 <=(width -1) && x0 >= 0 && y0 <= (height-1) && y0 >=0){
                    QColor color = QColor(image_data.pixel(x0,y0));
                    new_img.setPixel(x,y, qRgb(color.red(),color.green(),color.blue()));
                }

            }
        }
    }else if (sampling_method == IMAGE_BILINEAR_SAMPLING){
        // bilinear sampling
        for(int x = 0; x<new_width-1; x++){
            for(int y = 0;y<new_height-1; y++){
                // copy over

                double w0 = x/sx;
                double h0 = y/sy;

                int x1 = floor(w0);
                int x2 = x1+1;
                int y1 = floor(h0);
                int y2 = y1+1;
                double dx = w0-x1;
                double dy = h0-y1;

                if(w0 <(width -1) && w0 >= 0 && h0 < (height-1) && h0 >=0){

                  QColor colorx1y1 = QColor(image_data.pixel(x1, y1));
                  QColor colorx2y1 = QColor(image_data.pixel(x2, y1));
                  QColor colorx1y2 = QColor(image_data.pixel(x1, y2));
                  QColor colorx2y2 = QColor(image_data.pixel(x2, y2));

                  double aR = colorx1y1.red()*(1-dx) + colorx2y1.red()*dx;
                  double bR = colorx1y2.red()*(1-dx) + colorx2y2.red()*dx;
                  int destR = aR*(1-dy) +bR*dy;
                  double aG = colorx1y1.green()*(1-dx) + colorx2y1.green()*dx;
                  double bG = colorx1y2.green()*(1-dx) + colorx2y2.green()*dx;
                  int destG = aG*(1-dy) +bG*dy;
                  double aB = colorx1y1.blue()*(1-dx) + colorx2y1.blue()*dx;
                  double bB = colorx1y2.blue()*(1-dx) + colorx2y2.blue()*dx;
                  int destB = aB*(1-dy) +bB*dy;


                  new_img.setPixel(x,y, qRgb(destR,destG,destB));
                }
            }
        }
    }else if(sampling_method == IMAGE_GAUSSIAN_SAMPLING){
        // iterate
        for(int x = 0; x<new_width-1; x++){
            for(int y = 0;y<new_height-1; y++){
                // calculate the position of this pixle in old pic
                double w0 = x/sx;
                double h0 = y/sy;
                double sgm = 0.1;

                if(w0 <(width -1) && w0 >= 0 && h0 < (height-1) && h0 >=0){
                    //resample using Gausian
                    double resample_val_r = 0, norm_const= 0;
                    double resample_val_g = 0;
                    double resample_val_b = 0;
                    int r,g,b;
                    for(int i = 0; x < width; i++) {
                        for(int j = 0; y < height; j++) {
                            //norm const assures the weights all add up to 1
                            int gaus = exp(-0.5*pow(w0-i,2)/pow(sgm,2)-0.5*pow(h0-j,2)/pow(sgm,2));
                            norm_const += gaus;

                            (r,g,b) = image_data.pixel(i, j);
                            //gausian function
                            (resample_val_r,resample_val_g,resample_val_b) += (exp(-0.5*pow(w0-i,2)-0.5*pow(h0-j,2))) *(r,g,b) ;
                        }
                    }
                    //normalized
                    (r,g,b)= (resample_val_r,resample_val_g,resample_val_b) / norm_const;
                    new_img.setPixel(x,y, qRgb(r,g,b));
                }
            }
         }
    }
    image_data = new_img;
}

void Image::Sharpen()
{
    int filter_a [9]= {-1,-1,-1,
                       -1,9,-1,
                       -1,-1,-1};
        int sum = 1;
        int r,g,b;
        QColor color;
        QImage image_new(width,height,QImage::Format_RGB32);

        for(int x = 1; x < height-1; x++){
              for(int y = 1; y<width-1; y++){

                  r = 0;
                  g = 0;
                  b = 0;

                  for(int i = 0; i <= 2; i++){
                      for(int j = 0; j <= 2; j++){
                          color = QColor(image_data.pixel(y+i-1,x+j-1));
                          r += color.red()*(filter_a[(j*3+i)]);
                          g += color.green()*(filter_a[(j*3+i)]);
                          b += color.blue()*(filter_a[(j*3+i)]);
                      }
                  }

                  r = qBound(0, r/sum, 255);
                  g = qBound(0, g/sum, 255);
                  b = qBound(0, b/sum, 255);

                  image_new.setPixel(y,x, qRgb(r,g,b));
        }
    }
    image_data = image_new;
}
