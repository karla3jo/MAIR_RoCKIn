////////////////////////////////////////////////////////////////////////
//
// hello-world.cpp
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file and displays the result.
//
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <boost/lexical_cast.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

typedef std::map<std::string, std::pair<int, int> > ObjectMap;

//std::string path_dataset = "~/home/eriba/Datasets/256_ObjectCategories/";
std::string write_path = "../smv.xml";

void setImagesList(const std::string &path_dataset, const ObjectMap &map, std::vector<std::string> &list_paths);
void loadTrainingImages(const std::vector<std::string> &list_paths, std::vector<cv::Mat> &list_images);
void makeTrainingMatrix(const std::vector<cv::Mat> &list_images, cv::Mat &training_mat);
void makeLabelsMatrix(const ObjectMap &map, cv::Mat &labels_mat);
void trainSVM(cv::Ptr<CvSVM> svm, cv::Mat &training_mat, cv::Mat &labels);
void saveSVM(const cv::Ptr<CvSVM> svm, const std::string &write_path);


int main(int argc, char *argv[])
{

  std::cout << "Hello Computer Vision" << std::endl;

  if(argc<2)
  {
	std::cout << "Usage: ./object_training <dataset-path-name>" << std::endl;
	return -1;
  }

  std::string path_dataset = argv[1];

  ObjectMap map;

  // ["KEY"] = pair( int id, int num_images )
  map["tomato"] = std::make_pair(221, 103);
  map["spoon"] = std::make_pair(199, 105);

  int num_classes = map.size();

  std::vector<std::string> list_paths;
  setImagesList(path_dataset, map, list_paths);

  std::cout << "Set Images List OK" << std::endl;

  std::vector<cv::Mat> list_images;
  loadTrainingImages(list_paths, list_images);

  std::cout << "Load Training Images OK" << std::endl;

  cv::Mat training_mat;
  makeTrainingMatrix(list_images, training_mat);

  std::cout << "Make Training Matrix OK" << std::endl;

  cv::Mat labels = cv::Mat::zeros(training_mat.rows, num_classes, CV_8UC1);
  makeLabelsMatrix(map, labels);

  std::cout << "Make Labels Matrix OK" << std::endl;

  cv::Ptr<CvSVM> svm = cv::makePtr<CvSVM>();
  trainSVM(svm, training_mat, labels);

  std::cout << "Train SVM OK" << std::endl;

  saveSVM(svm, write_path);
  std::cout << "Save SVM OK" << std::endl << std::endl;


  std::cout << "Good Learning, BYE." << std::endl;

  return 0;

}

void setImagesList(const std::string &path_dataset, const ObjectMap &map, std::vector<std::string> &list_paths)
{
	for (ObjectMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		std::string key = it->first;
		int id = it->second.first;
		int num_images = it->second.second;

		std::string id_str;
		if(id < 10)
		{
			id_str = "00" + boost::lexical_cast<std::string>(id);
		}
		else if(id < 100)
		{
			id_str = "0" + boost::lexical_cast<std::string>(id);
		}
		else
		{
			id_str = boost::lexical_cast<std::string>(id);
		}


		for (int i = 1; i <= num_images; ++i)
		{
			std::string object_dir = id_str + "." + key + "/";

			std::string num_images_str;
			if(i < 10)
			{
				num_images_str = "000" + boost::lexical_cast<std::string>(i);
			}
			else if(i < 100)
			{
				num_images_str = "00" + boost::lexical_cast<std::string>(i);
			}
			else if(i < 1000)
			{
				num_images_str = "0" + boost::lexical_cast<std::string>(i);
			}
			else
			{
				num_images_str = boost::lexical_cast<std::string>(i);
			}

			std::string object_filename = id_str + "_" + num_images_str + ".jpg";
			std::string path = path_dataset + object_dir + object_filename;

			list_paths.push_back(path);
		}
	}
}

void loadTrainingImages(const std::vector<std::string> &list_paths, std::vector<cv::Mat> &list_images)
{
	std::vector<std::string>::const_iterator it = list_paths.begin();
	for ( ; it != list_paths.end(); ++it)
	{
		cv::Mat img = cv::imread(*it, cv::IMREAD_COLOR);

		// TODO: resize images

		list_images.push_back(img);
	}
}

void makeTrainingMatrix(const std::vector<cv::Mat> &list_images, cv::Mat &training_mat)
{
	// Set training image
	int num_files = list_images.size();
	int img_area = list_images[0].rows*list_images[0].cols;
	training_mat = cv::Mat(num_files,img_area,CV_32FC1);

	std::vector<cv::Mat>::const_iterator it = list_images.begin();
	for ( ; it != list_images.end(); ++it)
	{
		const cv::Mat & img = *it;

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

void makeLabelsMatrix(const ObjectMap &map, cv::Mat &labels_mat)
{
	int count = 0;
	int classes_count = 0;

	for (ObjectMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		std::string key = it->first;
		int id = it->second.first;
		int num_images = it->second.second;

		for (int i = 0; i < num_images; ++i)
		{
			labels_mat.at<uchar>(count, classes_count) = 1;
			count++;
		}
		classes_count++;
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
