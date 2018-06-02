#include <opencv2/opencv.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace ml;


struct Data
{
    img_t img;
    img_t samples;          //Set of train samples. Contains points on image
    img_t responses;        //Set of responses for train samples

    Data()
    {
        const int WIDTH = 841;
        const int HEIGHT = 594;
        img = img_t::zeros(HEIGHT, WIDTH, CC_8UC3);
        imshow("Train svmsgd", img);
    }
};

//Train with SVMSGD algorithm
//(samples, responses) is a train set
//weights is a required vector for decision function of SVMSGD algorithm
bool doTrain(const img_t samples, const img_t responses, img_t &weights, float &shift);

//function finds two points for drawing line (wx = 0)
bool findPointsForLine(const img_t &weights, float shift, Point points[], int width, int height);

// function finds cross point of line (wx = 0) and segment ( (y = HEIGHT, 0 <= x <= WIDTH) or (x = WIDTH, 0 <= y <= HEIGHT) )
bool findCrossPointWithBorders(const img_t &weights, float shift, const std::pair<Point,Point> &segment, Point &crossPoint);

//segments' initialization ( (y = HEIGHT, 0 <= x <= WIDTH) and (x = WIDTH, 0 <= y <= HEIGHT) )
void fillSegments(std::vector<std::pair<Point,Point> > &segments, int width, int height);

//redraw points' set and line (wx = 0)
void redraw(Data data, const Point points[2]);

//add point in train set, train SVMSGD algorithm and draw results on image
void addPointRetrainAndRedraw(Data &data, int x, int y, int response);


bool doTrain( const img_t samples, const img_t responses, img_t &weights, float &shift)
{
    Ptr<SVMSGD> svmsgd = SVMSGD::create();

    Ptr<TrainData> trainData = TrainData::create(samples, ml::ROW_SAMPLE, responses);
    svmsgd->train( trainData );

    if (svmsgd->isTrained())
    {
        weights = svmsgd->getWeights();
        shift = svmsgd->getShift();

        return true;
    }
    return false;
}

void fillSegments(std::vector<std::pair<Point,Point> > &segments, int width, int height)
{
    std::pair<Point,Point> currentSegment;

    currentSegment.first = Point(width, 0);
    currentSegment.second = Point(width, height);
    segments.push_back(currentSegment);

    currentSegment.first = Point(0, height);
    currentSegment.second = Point(width, height);
    segments.push_back(currentSegment);

    currentSegment.first = Point(0, 0);
    currentSegment.second = Point(width, 0);
    segments.push_back(currentSegment);

    currentSegment.first = Point(0, 0);
    currentSegment.second = Point(0, height);
    segments.push_back(currentSegment);
}


bool findCrossPointWithBorders(const img_t &weights, float shift, const std::pair<Point,Point> &segment, Point &crossPoint)
{
    int x = 0;
    int y = 0;
    int xMin = MIN(segment.first.x, segment.second.x);
    int xMax = MAX(segment.first.x, segment.second.x);
    int yMin = MIN(segment.first.y, segment.second.y);
    int yMax = MAX(segment.first.y, segment.second.y);

    CC_Assert(weights->tid == CC_32FC1);
    CC_Assert(xMin == xMax || yMin == yMax);

    if (xMin == xMax && weights.at<float>(1) != 0)
    {
        x = xMin;
        y = static_cast<int>(std::floor( - (weights.at<float>(0) * x + shift) / weights.at<float>(1)));
        if (y >= yMin && y <= yMax)
        {
            crossPoint.x = x;
            crossPoint.y = y;
            return true;
        }
    }
    else if (yMin == yMax && weights.at<float>(0) != 0)
    {
        y = yMin;
        x = static_cast<int>(std::floor( - (weights.at<float>(1) * y + shift) / weights.at<float>(0)));
        if (x >= xMin && x <= xMax)
        {
            crossPoint.x = x;
            crossPoint.y = y;
            return true;
        }
    }
    return false;
}

bool findPointsForLine(const img_t &weights, float shift, Point points[2], int width, int height)
{
    if (weights.empty())
    {
        return false;
    }

    int foundPointsCount = 0;
    std::vector<std::pair<Point,Point> > segments;
    fillSegments(segments, width, height);

    for (uint i = 0; i < segments.size(); i++)
    {
        if (findCrossPointWithBorders(weights, shift, segments[i], points[foundPointsCount]))
            foundPointsCount++;
        if (foundPointsCount >= 2)
            break;
    }

    return true;
}

void redraw(Data data, const Point points[2])
{
    data.img.setTo(0);
    Point center;
    int radius = 3;
    Scalar color;
    CC_Assert((data.samples->tid == CC_32FC1) && (data.responses->tid == CC_32FC1));
    for (int i = 0; i < data.samples->rows; i++)
    {
        center.x = static_cast<int>(data.samples.at<float>(i,0));
        center.y = static_cast<int>(data.samples.at<float>(i,1));
        color = (data.responses.at<float>(i) > 0) ? Scalar(128,128,0) : Scalar(0,128,128);
        circle(data.img, center, radius, color, 5);
    }
    line(data.img, points[0], points[1],Scalar(1,255,1));

    imshow("Train svmsgd", data.img);
}

void addPointRetrainAndRedraw(Data &data, int x, int y, int response)
{
    img_t currentSample(1, 2, CC_32FC1);

    currentSample.at<float>(0,0) = (float)x;
    currentSample.at<float>(0,1) = (float)y;
    data.samples.push_back(currentSample);
    data.responses.push_back(static_cast<float>(response));

    img_t weights(1, 2, CC_32FC1);
    float shift = 0;

    if (doTrain(data.samples, data.responses, weights, shift))
    {
        Point points[2];
        findPointsForLine(weights, shift, points, data.img->cols, data.img->rows);

        redraw(data, points);
    }
}


static void onMouse( int event, int x, int y, int, void* pData)
{
    Data &data = *(Data*)pData;

    switch( event )
    {
    case CC_EVENT_LBUTTONUP:
        addPointRetrainAndRedraw(data, x, y, 1);
        break;

    case CC_EVENT_RBUTTONDOWN:
        addPointRetrainAndRedraw(data, x, y, -1);
        break;
    }

}

int main()
{
    Data data;

    setMouseCallback( "Train svmsgd", onMouse, &data );
    waitKey();

    return 0;
}
