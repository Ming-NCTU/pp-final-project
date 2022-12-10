//
//  RPPG.hpp
//  Heartbeat
//
//  Created by Philipp Rouast on 7/07/2016.
//  Copyright © 2016 Philipp Roüast. All rights reserved.
//

#ifndef RPPG_hpp
#define RPPG_hpp

#include <fstream>
#include <string>
#include <opencv2/objdetect.hpp>
#include <opencv2/dnn.hpp>

#include <stdio.h>

using namespace cv;
using namespace dnn;
using namespace std;

enum rPPGAlgorithm { g, pca, xminay };
enum faceDetAlgorithm { haar, deep };

class RPPG {

public:

    // Constructor
    RPPG() {;}

    // Load Settings
    bool load(const rPPGAlgorithm rPPGAlg, const faceDetAlgorithm faceDetAlg,
              const int width, const int height, const double timeBase, const int downsample,
              const double samplingFrequency, const double rescanFrequency,
              const int minSignalSize, const int maxSignalSize,
              const string &logPath, const string &haarPath,
              const string &dnnProtoPath, const string &dnnModelPath,
              const bool log, const bool gui);

    void processFrame(Mat &frameRGB, Mat &frameGray, int time, double fps);

    void exit();

    typedef vector<Point2f> Contour2f;

private:

    void detectFace(Mat &frameRGB, Mat &frameGray);
    void setNearestBox(vector<Rect> boxes);
    void detectCorners(Mat &frameGray);
    void trackFace(Mat &frameGray);
    void updateMask(Mat &frameGray);
    void updateROI();
    void extractSignal_g();
    void extractSignal_pca();
    void extractSignal_xminay();
    // EWang dev multiple face
    //void m_extractSignal_g(int i);
    void m_extractSignal_pca(int i);
    //void m_extractSignal_xminay(int i);
    void m_estimateHeartrate(int i);
    void m_log(int i);
    //
    void estimateHeartrate();
    void draw(Mat &frameRGB);
    void invalidateFace();
    void log();

    // The algorithm
    rPPGAlgorithm rPPGAlg;

    // The classifier
    faceDetAlgorithm faceDetAlg;
    CascadeClassifier haarClassifier;
    Net dnnClassifier;

    // Settings
    Size minFaceSize;
    int maxSignalSize;
    int minSignalSize;
    double rescanFrequency;
    double samplingFrequency;
    double timeBase;
    bool logMode;
    bool guiMode;

    // State variables
    int64_t time;
    int high;
    int64_t lastSamplingTime;
    int64_t lastScanTime;
    int low;
    int64_t now;
    bool faceValid;
    bool rescanFlag;

    // Tracking
    Mat lastFrameGray;
    Contour2f corners;

    // Mask
    Rect box;
    Mat1b mask;
    vector<Mat1b> masks;
    Rect roi;

    // Raw signal
    Mat1d s;
    Mat1d t;
    Mat1b re;

    // Estimation
    Mat1d s_f;
    Mat1d bpms;
    Mat1d powerSpectrum;
    double bpm = 0.0;
    double fps;
    double frame_fps;
    double meanBpm;
    double minBpm;
    double maxBpm;

    // Logfiles
    ofstream logfile;
    ofstream logfileDetailed;
    string logfilepath;

    // Ewang Custom
    vector<Mat> maskedFrame;
    vector<Mat1d> ss;
    vector<Mat1d> ts;
    vector<Mat1d> res;
    vector<int> lows;
    vector<int> highs;
    vector<Mat1d> s_fs;
    vector<Mat1d> bpmses;
    vector<Mat1d> powerSpectrums;
    vector<double> meanBpms;
};


#endif /* RPPG_hpp */
