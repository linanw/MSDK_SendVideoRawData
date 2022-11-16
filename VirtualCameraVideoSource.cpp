#include "VirtualCameraVideoSource.h"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

int video_play_flag = -1;
int width = WIDTH;
int height = HEIGHT;

void PlayVideoFileToVirtualCamera(IZoomSDKVideoSender* video_sender, string video_source)
{
    char* frameBuffer;
    int frameLen = height / 2 * 3 * width;
    frameBuffer = (char*)malloc(frameLen);

    // execute in a thread.
    while (video_play_flag > 0 && video_sender) {
        Mat frame;
        VideoCapture cap;
        cap.open(video_source);
        if (!cap.isOpened()) {
            cerr << "ERROR! Unable to open camera\n";
            video_play_flag = 0;
            break;
        }
        else {
            //--- GRAB AND WRITE LOOP
            cout << "Start grabbing" << endl;
            while(video_play_flag > 0)
            {
                // wait for a new frame from camera and store it into 'frame'
                cap.read(frame);
                // check if we succeeded
                if (frame.empty()) {
                    cerr << "ERROR! blank frame grabbed\n";
                    break;
                }
                Mat resizedFrame;
                resize(frame, resizedFrame, Size(width, height), 0, 0, INTER_LINEAR);

                //covert Mat to YUV buffer
                Mat yuv;
                cv::cvtColor(resizedFrame, yuv, COLOR_BGRA2YUV_I420);
                char* p;
                for (int i = 0; i < height / 2 * 3; ++i) {
                    p = yuv.ptr<char>(i);
                    for (int j = 0; j < width; ++j) {
                        frameBuffer[i * width + j] = p[j];
                    }
                }
                ((IZoomSDKVideoSender*)video_sender)->sendVideoFrame(frameBuffer, width, height, frameLen, 0);
            }
            cap.release();
        }
    }
    video_play_flag = -1;
}

void VirtualCameraVideoSource::onInitialize(IZoomSDKVideoSender* sender, IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability& suggest_cap)
{
	cout << "onInitialize" << endl;
	video_sender_ = sender;
}

void VirtualCameraVideoSource::onPropertyChange(IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability suggest_cap)
{
	cout << "onPropertyChange" << endl;
    cout << "suggest frame: " << suggest_cap.frame << endl;
    cout << "suggest size: " << suggest_cap.width << "x" << suggest_cap.height << endl;
    width = suggest_cap.width;
    height = suggest_cap.height;
    cout << "calculated frameLen: " << height / 2 * 3 * width << endl;
}

void VirtualCameraVideoSource::onStartSend()
{
	cout << "onStartSend" << endl;
    if (video_sender_ && video_play_flag!=1){   
        while (video_play_flag > -1) {}
        video_play_flag = 1;
        thread(PlayVideoFileToVirtualCamera, video_sender_, video_source_).detach();
    }
}

void VirtualCameraVideoSource::onStopSend()
{
	cout << "onStopSend" << endl;
    video_play_flag = 0;
}

void VirtualCameraVideoSource::onUninitialized()
{
	cout << "onUninitialized" << endl;
	video_sender_ = nullptr;
}

VirtualCameraVideoSource::VirtualCameraVideoSource(string video_source)
{
    video_source_ = video_source;
}

