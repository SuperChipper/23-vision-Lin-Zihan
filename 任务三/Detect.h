//
// Created by zihan on 22-7-9.
//
/**
 *
 * @file Detect.h
 * @author 林梓涵
 * @version 1.0
 * @date 2022.7.14
 */
#ifndef DETECT_H
#define DETECT_H
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace std;
using namespace cv;
class Detect{
private:
    Mat RGBchannels[3];
    //储存三个颜色通道
    Mat Binary;
    //用于返回二值化结果
    Mat _struct;
    //形态学操作算子
    Mat erodeBinary;
    //形态学操作结果
    Mat _image;
    //储存图像结果
public:
    Detect(Mat frame);
    Mat Get();//返回图像结果
    Mat Blue_channel();//返回相应颜色通道
    Mat Red_channel();
    Mat Green_channel();
    Mat Binary_Blue();//返回相应颜色通道二值化的结果
    Mat Binary_Red();
    Mat Binary_Green();
    Mat Diff();//将三个通道颜色结合比较再二值化
    vector<vector<Point>> CompareContour(vector<vector<Point>>,vector<vector<Point>>);//轮廓矩比较（暂时不用）
    void Draw_rect(RotatedRect);//绘制矩形
};


class Rrect{
private:

    vector<RotatedRect> rrect;
    //最小外接矩形的集合
    vector<Point2f> rec_points;
    //疑似为装甲板轮廓的外接矩形的中心点
    vector<RotatedRect> critical_rect;
    //疑似为装甲板轮廓的外接矩形
    vector<Point2f> pnp_points;
    //储存解算角点的集合
public:
    Rrect(vector<vector<Point>> contour);//寻找最小外接矩形
    void find(Detect);//在矩形中找到装甲板并绘制
    void  PnP();
    void  PnP_init();

};

void draw_side(RotatedRect,Mat);//



void Detector(Mat);//检测图像

void Detect_target(string,string,int);//根据文件类型调用检测函数

#endif DETECT_H
