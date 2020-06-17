#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <algorithm>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>

static const auto HUE_MIN = 0u;
static const auto HUE_MAX = 180u;
static const auto SATURATION_MIN = 5u;
static const auto SATURATION_MAX = 160u;
static const auto VALUE_MIN = 175u;
static const auto VALUE_MAX = 255u;

static const auto BLUR_FACTOR = 8u;

//Size bounds for rectangles
static const auto MIN_WIDTH = 20u;
static const auto MAX_WIDTH = 200u;
static const auto MIN_HEIGHT = 10u;
static const auto MAX_HEIGHT = 100u;

static std::vector<std::string> pictureNames;

void Blur(cv::Mat& image){
	for (unsigned int i = 1u; i < BLUR_FACTOR; i += 2u){
		cv::blur(image, image, cv::Size(i, i), cv::Point(-1, -1), cv::BORDER_CONSTANT);
	}
}

void ConvertToHSV(cv::Mat& image){
	cv::cvtColor(image, image, cv::COLOR_BGR2HSV);
}

void FilterRange(cv::Mat& image){
	cv::inRange(image, cv::Scalar(HUE_MIN, SATURATION_MIN, VALUE_MIN), cv::Scalar(HUE_MAX, SATURATION_MAX, VALUE_MAX), image);
}

int main(){
	auto minWidth = MAX_WIDTH;
	auto maxWidth = MIN_WIDTH;
	auto minHeight = MAX_HEIGHT;
	auto maxHeight = MIN_HEIGHT;

	//Read file names from images/list.txt
	std::ifstream myfile("images/list.txt");
	if(myfile.is_open()){
		std::string line;
		while( getline(myfile,line) ) {
			pictureNames.push_back(line);
		}
	}else{
		std::cout<<"Could not get file\n";
		return -1;
	}

	std::map<std::string, cv::Rect> finalRectangles;

	for (const auto& pictureName : pictureNames)
	{
		//Reads images from list.txt in images
		cv::Mat image;
		image = cv::imread(pictureName);

		//We then blur, convert to HSV, and then filter by HSV
		Blur(image);
		ConvertToHSV(image);
		FilterRange(image);
		//Image is now binary
	
		//Find all the pieces/contours that are left from the inRange
		std::vector<std::vector<cv::Point> > contours;
		cv::findContours(image, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

		//Make the vector contours.size large (so we don't keep having to grow over and over
		std::vector<cv::Rect> boundingRectangles;
		boundingRectangles.reserve(contours.size());

		//Make a boundingRect for each contour, and put it in bounginRectangles
		for (const auto& contour : contours)
		{	
			boundingRectangles.emplace_back(cv::boundingRect(cv::Mat(contour)));
		}

		//Sort the boundingRects and get the largest
		std::sort(boundingRectangles.begin(), boundingRectangles.end(), [](const cv::Rect& lhs, const cv::Rect& rhs)
		{
			return lhs.area() > rhs.area();
		});
		

		std::cout << pictureName << ": "<<std::endl;

		for (const auto& rectangle : boundingRectangles)
		{
			if (rectangle.width >= MIN_WIDTH && rectangle.width <= MAX_WIDTH &&
			    rectangle.height >= MIN_HEIGHT && rectangle.height <= MAX_HEIGHT)
			{
				std::cout << "\ty: " << rectangle.y << std::endl
					  << "\tx: " << rectangle.x << std::endl
				          << "\twidth: " << rectangle.width << std::endl
				          << "\theight: " << rectangle.height << std::endl << std::endl;

				minWidth = std::min(minWidth, static_cast<unsigned int>(rectangle.width));
				maxWidth = std::max(maxWidth, static_cast<unsigned int>(rectangle.width));
				minHeight = std::min(minHeight, static_cast<unsigned int>(rectangle.height));
				maxHeight = std::max(maxHeight, static_cast<unsigned int>(rectangle.height));
			}
		}
		std::cout << std::endl;
	}
	
	std::cout << "Min Width: " << minWidth << std::endl
	          << "Max Width: " << maxWidth << std::endl
	          << "Min Height: " << minHeight << std::endl
	          << "Max Height: " << maxHeight << std::endl << std::endl;
}

