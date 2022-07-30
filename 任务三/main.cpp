/**
 *
 * @file main.cpp
 * @author 林梓涵
 * @version 1.0
 * @date 2022.7.14
 */
#include "Detect.h"

using namespace std;
using namespace cv;
int main(int argc, char *argv[]) {
    Detect_target("video","../1.avi",60);
    destroyAllWindows();
    return 0;
}