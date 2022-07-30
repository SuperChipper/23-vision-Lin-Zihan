/**
 *
 * @file Detect.cpp
 * @author 林梓涵
 * @version 1.0
 * @date 2022.7.14
 */
#include "Detect.h"

/**
 *
 * @param frame 传入图像帧
 */
Detect::Detect(Mat frame){

const Mat _struct= getStructuringElement(1,Size(3,3));
_image=frame;
split(_image,RGBchannels);
    split(_image,RGBchannels);
};
Mat Detect::Get() {
    return _image;
}
/**
 *
 * @return 返回图像的对应颜色通道
 */
Mat Detect::Blue_channel(){
    return RGBchannels[0];
}
Mat Detect::Red_channel(){
    return RGBchannels[2];
}
Mat Detect::Green_channel(){
    return RGBchannels[1];
}
/**
 *
 * @return 返回二值化结果
 */
Mat Detect::Binary_Blue(){
    threshold(Blue_channel(),Binary,230,255,THRESH_BINARY);
    morphologyEx(Binary,erodeBinary,MORPH_CLOSE,_struct,Point(-1,-1),1);
    return Binary;
}
Mat Detect::Binary_Red(){
    threshold(Red_channel(),Binary,125,255,THRESH_BINARY);
    morphologyEx(Binary,erodeBinary,MORPH_CLOSE,_struct,Point(-1,-1),2);
    return erodeBinary;
}
Mat Detect::Binary_Green() {
    threshold(Green_channel(),Binary,125,255,THRESH_BINARY);
    return Binary;
}
/**
 * @brief 综合各个通道的颜色来进行二值化
 *
 * @return 返回图像的二值化结果
 */
Mat Detect::Diff(){
    threshold(0.1*Blue_channel()+0.5*Green_channel()+0.4*Red_channel(),Binary,190,255,THRESH_BINARY);

    morphologyEx(Binary,erodeBinary,MORPH_DILATE,_struct,Point(-1,-1),1);
    return erodeBinary;
};
/**
 *
 * @param contourR 传入相似的两个轮廓
 * @param contourB
 * @return 比较后相似的轮廓
 */
vector<vector<Point>> Detect::CompareContour(vector<vector<Point>> contourR,vector<vector<Point>>contourB) {//预期返回一个向量包含比较后相似的轮廓
    vector<vector<Point>> contour;
    for (int n=0;n<contourR.size();n++){
        Moments mm= moments(contourR[n]);
        Mat hum;
        HuMoments(mm,hum);
        double dist;
        for (int i=0;i<contourB.size();i++){
            Moments mm2=moments(contourB[i]);
            Mat hu2;
            HuMoments(mm2,hu2);
            dist = matchShapes(hum,hu2,CONTOURS_MATCH_I1,0);
            if(dist<1){
                //drawContours(img,contourR,n,Scalar(0,0,255),3,8);
                contour.push_back(contourB[i]);//末尾添加轮廓
            }
        }
    }
    return contour;
}
/**
 * @brief 绘制矩形
 *
 * @param rrect 旋转矩形 Rotatedrect
 */
void Detect::Draw_rect(RotatedRect rrect) {
    Mat img=_image;
    Point2f points[4];
    rrect.points(points);
    Point2f cent=rrect.center;
    for(int i=0;i<4;i++){
        if(i==3)
        {
            line(img,points[i],points[0],Scalar(0,0,255),2,8,0);
            break;
        }
        line(img,points[i],points[i+1],Scalar(0,0,255),2,8,0);

    }
    circle(img,cent,2,Scalar(200,150,0),2,8,0);
}
/**
 *
 * @param img 传入图像
 *
 * @brief 图像检测函数封装
 *
 */
void Detector(Mat img){
    namedWindow("img", WINDOW_FREERATIO);
    namedWindow("bin", WINDOW_FREERATIO);
    Detect image(img);//初始化，传入图像
    Mat bin;
    vector<vector<Point>> contour;
    vector<Vec4i> hierarchy1;
    bin = image.Diff();
    findContours(bin, contour, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<RotatedRect> rrect;
    Rrect rec(contour);
    rec.find(image);
    imshow("bin", bin);
    imshow("img", image.Get());
}
/**
 *
 * @param type 检测类型，分为video和image
 * @param path 检测的文件的路径
 * @param waitkey 每一帧等待时间毫秒
 *
 * @brief 核心检测函数,根据文件类型调用检测函数
 *
 */
void Detect_target(string type,string path,int waitkey){
    Mat img;
    if(type=="video"){
        VideoCapture video(path);
        while(1){
            video>>img;
            if(img.empty()){
                break;
            }
            Detector(img);
            waitKey(waitkey);
        }
    }
    else if(type=="image"){
        img=imread(path);
        if (!img.empty()) {
            Detector(img);
            waitKey(waitkey);
        }

    }
    else
        cout<<"Error, invalid type, please insert \"video\" or \"image\"";
}
/**
 *
 * @param contour 轮廓
 *
 * @brief 根据轮廓寻找最小外接矩形
 */
Rrect::Rrect(vector<vector<Point>> contour) {
    for (auto &cont: contour) {
        Point2f p[4];
        RotatedRect r = minAreaRect(cont);
        rrect.push_back(r);
/*      r.points(p);
        points.push_back(p);
        for(auto&point:p){
            cout<<point;
        }
        cout<<endl;
*/
    }
}

void Rrect::find(Detect image) {
    for (auto &rec: rrect) {
//if (((rec.angle > 40) && (rec.angle < 140)) || ((rec.angle > 220) && (rec.angle < 320))) {
            //if ((rec.size.width / rec.size.height > 1.9) && (rec.size.width / rec.size.height < 16)) {
                for (auto &cri: critical_rect) {
                    Point2f ptr = cri.center;
                    if (((ptr.x - rec.center.x) > 2* rec.size.width) &&

                        ((ptr.y - rec.center.y == 0) || (abs(ptr.x - rec.center.x) / abs(ptr.y - rec.center.y) > 3))
                        &&(abs(cri.angle-rec.angle)<11)) {
                        if((rec.size.width>0.6*cri.size.width)&&(0.6*rec.size.width<cri.size.width))
                        {
                            line(image.Get(), ptr, rec.center, Scalar(100, 200, 255), 2, 8, 0);;
                            circle(image.Get(), (rec.center + ptr) / 2, 2, Scalar(255, 20, 255), 2, 8, 0);
                            draw_side(rec,image.Get());
                            draw_side(cri,image.Get());
                        }
                    }
                }
                critical_rect.push_back(rec);
                rec_points.push_back(rec.center);
                //cout<<rec.size.width<<"\t"<<rec.size.height<<endl;
                image.Draw_rect(rec);
            //}
        }
    //}
}

void Rrect::PnP_init() {
    for(auto&rect:rrect){
        Point2f points[4];
        rect.points(points);
        Point2f point1[2]={0.5*(points[0]+points[1]),0.5*(points[2]+points[3])};
        pnp_points.push_back(point1[0]);
        pnp_points.push_back(point1[1]);
    }
}
void draw_side(RotatedRect rect,Mat image){
    Point2f points[4];
    rect.points(points);
    Point2f point1=0.5*(points[0]+points[1]),point2=0.5*(points[2]+points[3]);
    circle(image, point1, 2, Scalar(255, 120, 255), 2, 8, 0);
    circle(image, point2, 2, Scalar(255, 120, 255), 2, 8, 0);

}
void Pnp(Mat img){
    namedWindow("img", WINDOW_FREERATIO);
    //namedWindow("bin", WINDOW_FREERATIO);
    Detect image(img);//初始化，传入图像
    Mat bin;
    vector<vector<Point>> contour;
    vector<Vec4i> hierarchy1;
    bin = image.Diff();
    findContours(bin, contour, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<RotatedRect> rrect;
    Rrect rec(contour);
    //rec.find(image);
    //imshow("bin", bin);
    imshow("img", image.Get());
}