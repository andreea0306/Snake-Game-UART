////////////////////////////////////////////////////////////////////////////////
// author: Andreea Andrei
// file name: Snake.cpp
// description: Snake class - creates the snake & moves & grows & draw on map
///////////////////////////////////////////////////////////////////////////////

#include "Defines.h"
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

class Snake {
private:
    int direction;
public:
    std::vector<cv::Point> body;

    Snake(int x, int y) {
        direction = 0; // start moving to the right
        body.emplace_back(x, y); // add the head of the snake
        body.emplace_back(x-GRID_SIZE, y);
        body.emplace_back(x-2*GRID_SIZE, y); // tail
    }

    void draw(cv::Mat& image) {
        for (int i = 1; i < body.size(); i++) {
            rectangle(image, cv::Rect(body[i].x, body[i].y, GRID_SIZE, GRID_SIZE), cv::Scalar(0, 0, 0), -1);
        }
    }

    void move() {
        cv::Point head = body[0];
        // right
        if (direction == 0) {
            head.x += GRID_SIZE;
        }
        // down
        else if (direction == 1) {
            head.y += GRID_SIZE;
        }
        // left
        else if (direction == 2) {
            head.x -= GRID_SIZE;
        }
        // up
        else if (direction == 3) {
            head.y -= GRID_SIZE;
        }
        body.insert(body.begin(), head); // move head to the new point
        body.pop_back(); // pop the tail
    }

    void set_direction(int new_direction) {
        direction = new_direction;
    }

    bool check_collision() {
        cv::Point head = body[0];
        // check collision with walls
        if (head.x < 0 || head.x >= WIDTH+GRID_SIZE || head.y < 0 || head.y >= HEIGHT+GRID_SIZE) {
            return true;
        }
        // check collision with body
        if (head.x == get_tail().x && head.y == get_tail().y) {
            return true;
        }

        return false;
    }

    cv::Point get_tail() {
        return body[body.size()];
    }

    void grow() {
        body.push_back(get_tail());
    }

    int getDirection() {
        return direction;
    }
};