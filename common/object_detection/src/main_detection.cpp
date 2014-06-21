////////////////////////////////////////////////////////////////////////
//
// hello-world.cpp
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file and displays the result. 
//
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


int main(int argc, char *argv[])
{

  std::cout << "Hello Computer Vision" << std::endl;

  cv::Mat img; 

  if(argc<2)
  {
	std::cout << "Usage: ./object_detection <image-file-name>" << std::endl;
    return -1;
  }

  // load an image  
  img = cv::imread(argv[1]);

  if(img.empty())
  {
    std::cout << "Could not load image file: " << argv[1] << std::endl;
    return -1;
  }

  // create a window
  cv::namedWindow("Main Window", cv::WINDOW_KEEPRATIO);

  // show the image
  cv::imshow("Main Window", img );

  // wait for a key
  cv::waitKey(0);

  // close a window
  cv::destroyWindow("Main Window");

  return 0;

}
