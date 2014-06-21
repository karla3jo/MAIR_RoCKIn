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
#include <opencv2/ml/ml.hpp>


int main(int argc, char *argv[])
{

  std::cout << "Hello Computer Vision" << std::endl;

  cv::Mat img; 

  if(argc<2)
  {
	std::cout << "Usage: ./object_training <image-file-name>" << std::endl;
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

void loadTrainingImages(const std::string &path, std::vector<cv::Mat> &list_images)
{

	cv::FileStorage storage(path, cv::FileStorage::READ + + cv::FileStorage::MEMORY);

	cv::Mat list_names_mat;
	std::vector<std::string> list_names;

	storage["list_names"] >> list_names_mat;
	storage.release();

	list_names_mat.copyTo(list_images);

	for (int i = 0; i < list_images.size(); ++i)
	{
		cv::Mat img = cv::imread(list_names[i], cv::IMREAD_COLOR);
		list_images.push_back(img);
	}

}

void makeTrainingMatrix(const std::vector<cv::Mat> &list_images, cv::Mat &training_mat)
{
	// Set training image
	int num_files = 5;
	int img_area = 4*3;
	training_mat = cv::Mat(num_files,img_area,CV_32FC1);


	for (int i = 0; i < list_images.size(); ++i)
	{
		const cv::Mat & img = list_images[i];

		// TODO: scaling, rotation, gradients, etc ...
		//cv::Mat img_scaled;
		//cv::Mat img_rotated;
		//cv::Mat img_gradients;

		// Convert image to vector
		cv::Mat img_vec = img.reshape(1, img.cols*img.cols);


		// Add image to training matrix
		training_mat.push_back(img_vec);

	}

}

void trainSVM(cv::Ptr<CvSVM> svm, cv::Mat &training_mat, cv::Mat &labels)
{
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::POLY;
	params.gamma = 3;

	svm->train(training_mat, labels, cv::Mat(), cv::Mat(), params); // training
}

void saveSVM(const cv::Ptr<CvSVM> svm, const std::string &write_path)
{
	svm->save(write_path.c_str()); // saving
}

void loadSVM(const cv::Ptr<CvSVM> svm, const std::string &read_path)
{
	svm->load(read_path.c_str()); // loading
}
