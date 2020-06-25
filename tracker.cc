#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
	// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()
int main( int argc, char** argv) {

    // Vector of tracker types
    std::string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD", "CSRT"};
    
    // Create a tracker
    int trackerType = 4;

    cv::Ptr<cv::Tracker> tracker;

    #if (CV_MINOR_VERSION < 3)
    tracker = cv::Tracker::create(trackerType);
    #else
    switch(trackerType) {
        case 0:
        tracker = cv::TrackerBoosting::create();
        break;
        case 1:
        tracker = cv::TrackerMIL::create();
        break;
        case 2:
        tracker = cv::TrackerKCF::create();
        break;
        case 3:
        tracker = cv::TrackerTLD::create();
        break;
        case 4:
        tracker = cv::TrackerCSRT::create();
        break;
    }
    #endif

    cv::Mat frame;
    std::vector<std::vector<cv::Point>> contours;
    cv::VideoCapture video(0);

    bool ok = video.read(frame);
    cv::Rect bbox1 = cv::selectROI("Live Video Stream", frame, false);
    cv::Rect2d bbox(bbox1.x, bbox1.y, bbox1.width, bbox1.height);
    std::cout << "Rect Matrix: " << bbox1 << std::endl << "Rect2d Matrix: " << bbox << std::endl;
    // cv::rectangle(frame, bbox, cv::Scalar(0, 255, 0), 2, 1);
    cv::imshow("Live Video Stream" , frame);
    tracker->init(frame, bbox);

    if ( video.isOpened() ) {
        while ( video.read(frame) ) {
            bool ok = tracker->update(frame, bbox);

            if (ok) {
                cv::rectangle(frame, bbox, cv::Scalar(0, 255, 0), 2, 1);
            }
            cv::imshow("Live Video Stream", frame);
            int key = cv::waitKey(1);
            if ( key == 27 ) {
                video.release();
                cv::destroyAllWindows();
                break;
                exit(0);
            }
        }
    }
    else {
        std::cout << "Unable to access webcam " << std::endl;
    }
    return 0;
}