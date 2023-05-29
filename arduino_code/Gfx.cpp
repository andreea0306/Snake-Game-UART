///////////////////////////////////////////////////////////////////////////////////
// author: Andreea Andrei
// file name: Gfx.cpp
// description: Graphics class - generates the pattern for map and menu windows
//////////////////////////////////////////////////////////////////////////////////

#include "Defines.h"
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Gfx {
private:
    int width;
    int height;
    int grid_size;
public:
    Gfx(int w, int h, int gs) {
        width = w;
        height = h;
        grid_size = gs;
    }
    [[nodiscard]] cv::Mat getGreenChessMap() const {
        // add default color a green to the base image
        cv::Mat greenChessMap(height, width, CV_8UC3, cv::Scalar(10,220,10));
        // over the base color add darker green squares
        for(int i=0;i<width;i++) {
            for(int j=0;j<height;j++) {
                if((i+j)%2 == 0) {
                    cv::rectangle(greenChessMap,cv::Point(i*grid_size,j*grid_size), cv::Point((i+1)*grid_size, (j+1)*grid_size), cv::Scalar(40,200,60),-1);
                }
            }
        }
        return greenChessMap;
    }
    static cv::Mat blendImages(cv::Mat image1, cv::Mat image2, double alphaWeight, double betaWeight) {
        cv::Mat blendedImage;
        cv::addWeighted(image1, alphaWeight, image2, betaWeight, 0.0, blendedImage);
        return blendedImage;
    }
};