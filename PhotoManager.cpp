#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

#define cameraID 0

int main(){
	cv::namedWindow("Display window",cv::WINDOW_AUTOSIZE); //Create a window for display
	cv::Mat image;
	cv::VideoCapture camera;

	std::ostringstream ss;
	std::ofstream imageList;
	imageList.open("images/list.txt");
	
	time_t current_time;
	time_t past_time;
	int amountInARow = 1;

	if (camera.open(cameraID))
	{
		while(true){
			camera >> image;
			imshow("Display window",image);
			
			//If user hits enter, write the image to images/time.jpg
			//I also add a number of times I have written an image during
			//that secound in parentheses
			if(cv::waitKey(0) == 13){
				current_time = time(0);	
				ss.str(""); //Clears ss
				ss << "images/";
				ss << "image_taken_at_";
				ss << current_time;
				
				//If an image has already been written this second
				if(past_time == current_time){
					ss << "(" << amountInARow << ")";
					amountInARow++;
				}else{
					amountInARow = 1;
				}
				ss << ".jpg";
				past_time = current_time;

				std::cout<<"Saved image "<<ss.str()<<"\n";	
				cv::imwrite(ss.str(), image);
				imageList << ss.str()+"\n";
				imageList.flush();
			}
		}
	}
}
