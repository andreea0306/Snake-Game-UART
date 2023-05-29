#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
 // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include<opencv2/opencv.hpp>
#include<opencv2/core/mat.hpp>
#include <thread>

#include "Defines.h"
#include "Snake.cpp"
#include "Food.cpp"
#include "Gfx.cpp"


void displayGame(Snake snake, Food food, Gfx gfx) {
    // draw the game window
    namedWindow("Game", cv::WINDOW_NORMAL);
    // create a fixed size window
    cv::resizeWindow( "Game", WIDTH, HEIGHT);
    cv::Mat gameImage = gfx.getGreenChessMap();
    // draw snake on game window
    snake.draw(gameImage);
    // draw food on game window
    food.draw(gameImage);
    imshow("Game", gameImage);
    cv::waitKey(1);
    unsigned int microsecond = 100000;
    usleep(3  * microsecond);//sleeps for 3 second
}

int main() {
// Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    int serialPort = open("/dev/ttyUSB1", O_RDWR /*| O_NOCTTY | O_NONBLOCK*/);
    //  debug
    if(serialPort == -1) {
        printf("Error %i : %s\n", errno, strerror(errno));
        return 1;
    }
    //  std::cout<<serialPort<<std::endl;
    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if(tcgetattr(serialPort, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }


    // Set serial port settings (baud rate, data bits, etc.)
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in baud rate to be 115200
    cfsetispeed(&tty, B115200);

    // initialize the game variables
    Snake snake(18, 18);
    Food food;
    int score = 0;
    Gfx gfx = Gfx(WIDTH, HEIGHT, GRID_SIZE);
    std::string menu_file_path = "/home/andreea/CLionProjects/newSnakeTry/snake_start_menu.png";
    // define the weight of each image in the blend
    double alpha = 0.5;
    double beta = 0.1;
    // add an image to the menu and resize it to fit with the initial image
    cv::Mat snakeImg = cv::imread(menu_file_path);
    cv::Mat resizedSnakeImg;
    cv::resize(snakeImg, resizedSnakeImg,cv::Size(WIDTH, HEIGHT), cv::INTER_LINEAR);
        // run the game loop
        while (true) {
            std::thread displayThread(displayGame, snake, food, gfx);
            displayThread.join();
            //  get user input from M5StickC Plus
            char buffer;
            size_t bytesRead = read(serialPort, &buffer, sizeof(buffer));
            std::cout <<"buffer: "<<buffer <<" and bytes read:"<<bytesRead<<std::endl;
            if (buffer == '3' && snake.getDirection() != 1) {
                snake.set_direction(3);
                std::cout <<"direction 3 set"<< std::endl;
            } else if (buffer == '2' && snake.getDirection() != 0) {
                snake.set_direction(2);
                std::cout <<"direction 2 set"<< std::endl;
            } else if (buffer== '1' && snake.getDirection() != 3) {
                snake.set_direction(1);
                std::cout <<"direction 1 set"<< std::endl;
            } else if (buffer == '0' && snake.getDirection() != 2) {
                snake.set_direction(0);
                std::cout <<"direction 0 set"<< std::endl;
            }
            snake.move();
            // check collision with food and body
            if (snake.check_collision()) {
                std::cout<<"am intrat in check collision"<<std::endl;
                // create the Game Over window
                cv::Mat scoreImage = gfx.getGreenChessMap();
                scoreImage = gfx.blendImages(scoreImage, resizedSnakeImg, alpha, beta);
                // display final score
                cv::putText(scoreImage, "GAME OVER!", cv::Point(270, 50), cv::FONT_HERSHEY_DUPLEX, 1.0,
                            cv::Scalar(0, 0, 0), 2);
                std::string s = std::to_string(score);
                cv::putText(scoreImage, "Score: " + s, cv::Point(0, 0), cv::FONT_HERSHEY_DUPLEX, 1.0,
                            cv::Scalar(0, 0, 0), 2);
                std::cout<<"final score: "<<score<<std::endl;
                cv::destroyWindow("Game");
                imshow("Over", scoreImage);
                cv::waitKey(0);
                break;
            } else if (snake.body[0] == food.get_position()) {
                // if snake ate the food, a new apple is generated, score increases and snake grows
                food = Food();
                score++;
                snake.grow();
            }
        }
        close(serialPort);
//    }
    return 0;
}
