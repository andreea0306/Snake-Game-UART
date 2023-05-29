////////////////////////////////////////////////////////////////////////////
// author: Andreea Andrei
// file name: Food.cpp
// description: Snake's food class - generates apple position & draw it
///////////////////////////////////////////////////////////////////////////
#include "Defines.h"
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Food {
private:
    cv::Point position;
public:
    // generate a random pair fo coordinates for apple
    Food(){
        position.x = (rand() % (WIDTH/GRID_SIZE)) * GRID_SIZE;
        position.y = (rand() % (HEIGHT/GRID_SIZE)) * GRID_SIZE;
    }
    // draw apple on map
    void draw(cv::Mat& image) const {
        rectangle(image, cv::Rect(position.x, position.y, GRID_SIZE, GRID_SIZE), cv::Scalar(0, 0, 255), -1);
    }
    // get apple's position
    cv::Point get_position() {
        return position;
    }
};