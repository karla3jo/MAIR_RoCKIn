/**
* @file main_training.cpp
* @brief Main program for training a SVM using a given data set
* @date Jun 22, 2014
* @author Edgar Riba
* @author Claudi Ruiz
*/

#include <iostream>
#include <boost/lexical_cast.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/contrib/contrib.hpp>

typedef std::map<std::string, std::pair<int, int> > ObjectMap;


/* ************************************************************************* */

// Algorithm parameters
std::string path_dataset = "/home/eriba/Datasets/256_ObjectCategories/";
std::string write_path = "../smv.xml";


/* ************************************************************************* */

/**
* @brief This function parses the images from a data set and its labels to a vector
* @param path_dataset - Path for the data set in your local machine
* @param map - Map with class name, id and number of samples
* @param image - Vector to store the images of the data set
* @param labels - Vector to store the labels of the objects classes
*/
void loadData( const std::string &path_dataset, const ObjectMap &map,
		       std::vector<cv::Mat> &images,
		       std::vector<int> &labels);

/**
* @brief This function extract the images features and put them in a row vector
* @param image - Vector with the data set images
* @param training_mat - Matrix with images features in the following format [numSamples x numFeatures]
*/
void makeTrainingMatrix(const std::vector<cv::Mat> &images, cv::Mat &training_mat);

/**
* @brief This function convert into PCA space the training matrix and train the SVM
* @param svm - SVM pointer to the SVM to be trained
* @param training_mat - Matrix with images features in the following format [numSamples x numFeatures]
* @param labels_mat - Matrix with images labels in the following format [numSamples x 1]
*/
void trainSVM(cv::Ptr<CvSVM> svm, cv::Mat &training_mat, cv::Mat &labels_mat);

/**
* @brief This function save the SVM into a *.xml file
* @param svm - SVM pointer to the trained SVM
* @param write_path - Path for the XML file in your local machine
*/
void saveSVM(const cv::Ptr<CvSVM> svm, const std::string &write_path);


/* ************************************************************************* */
int main(int argc, char *argv[])
{

  std::cout << "Hello Computer Vision" << std::endl;

  /*if(argc<2)
  {
	std::cout << "Usage: ./object_training <dataset-path-name>" << std::endl;
	return -1;
  }

  std::string path_dataset = argv[1];*/

  cv::TickMeter tm;

  //
  // Set the Name class, the id and the number of samples
  // Example: map["tomato"] = std::make_pair(1, 100);
  //

  ObjectMap map;

  map["tomato"] = std::make_pair(221, 103);
  map["spoon"] = std::make_pair(199, 105);


  // START THE ALGORITHM

  // -- Step 1: Load the images and set labels
  std::vector<cv::Mat> images;
  std::vector<int> labels;

  tm.reset(); tm.start();
  loadData(path_dataset, map, images, labels);
  tm.stop();

  std::cout << "Load the images and set labels OK " << tm.getTimeMilli() << std::endl;


  // -- Step 2: Build the training matrix
  cv::Mat training_mat;
  tm.reset(); tm.start();
  makeTrainingMatrix(images, training_mat);
  tm.stop();

  std::cout << "Make Training Matrix OK " << tm.getTimeMilli() << std::endl;


  // -- Step 3: Build the labels matrix
  tm.reset(); tm.start();
  cv::Mat labels_mat(labels);
  tm.stop();

  std::cout << "Make Labels Matrix OK " << tm.getTimeMilli() << std::endl;


  // -- Step 4: Train the SVM
  cv::Ptr<CvSVM> svm = cv::makePtr<CvSVM>();
  tm.reset(); tm.start();
  trainSVM(svm, training_mat, labels_mat);
  tm.stop();

  std::cout << "Train SVM OK " << tm.getTimeMilli() << std::endl;


  // -- Step 4: Save the SVM
  saveSVM(svm, write_path);
  std::cout << "Save SVM OK" << std::endl << std::endl;


  std::cout << "Good Learning, BYE." << std::endl;

  return 0;

}


/* ************************************************************************* */
void loadData(const std::string &path_dataset, const ObjectMap &map, std::vector<cv::Mat> &images, std::vector<int> &labels)
{
	int count_classes = 1;

	for (ObjectMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		std::string key = it->first;
		int id = it->second.first;
		int num_images = it->second.second;

		// parser image id
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


		// parser image filename
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

			// add images and labels
			images.push_back(cv::imread(path, cv::IMREAD_COLOR));
			labels.push_back(count_classes);
		}
		count_classes++;
	}
}

/* ************************************************************************* */
void makeTrainingMatrix(const std::vector<cv::Mat> &images, cv::Mat &training_mat)
{

	std::vector<cv::Mat>::const_iterator it = images.begin();
	for ( ; it != images.end(); ++it)
	{
		const cv::Mat & img = *it;

		// resize image
		cv::Mat img_resized;
		cv::resize(img.clone(), img_resized, cv::Size(50,50));

		// extract RGB channels
		std::vector<cv::Mat> channels;
		cv::split(img_resized, channels);

		// convert RGB to gray
		cv::Mat img_gray;
		cv::cvtColor(img_resized.clone(), img_gray, cv::COLOR_RGB2GRAY);

		// extract Canny edges
		cv::Mat img_canny;
	    cv::Canny(img_gray, img_canny, 10, 350);

		// TODO: scaling, rotation, gradients, etc ...
		//cv::Mat img_scaled;
		//cv::Mat img_rotated;
		//cv::Mat img_gradients;

		// Convert image to vector
		cv::Mat img_vec = img_gray.clone().reshape(1, 1);
		//std::cout << img_vec.type() << std::endl;

		// Add image vector to training matrix
		training_mat.push_back(img_vec);

	}

	CV_Assert(training_mat.rows == images.size());
}

/* ************************************************************************* */
void trainSVM(cv::Ptr<CvSVM> svm, cv::Mat &training_mat, cv::Mat &labels_mat)
{
	CV_Assert(training_mat.rows == labels_mat.rows);

	cv::SVMParams params;
	params.svm_type = cv::SVM::C_SVC;
	params.kernel_type = cv::SVM::POLY;
	params.gamma = 20;
	params.degree = 3;
	params.coef0 = 0;

	params.C = 7;
	params.nu = 0.0;
	params.p = 0.0;

	params.class_weights = NULL;
	params.term_crit.type = CV_TERMCRIT_ITER + CV_TERMCRIT_EPS;
	params.term_crit.max_iter = 1000;
	params.term_crit.epsilon = 1e-6;

	// Number of components to keep for the PCA:
	int num_components = 30;

	// Perform a PCA:
	cv::PCA pca(training_mat, cv::Mat(), cv::PCA::DATA_AS_ROW, num_components);

	// And copy the PCA results:
	cv::Mat mean = pca.mean.clone();
	cv::Mat eigenvalues = pca.eigenvalues.clone();
	cv::Mat eigenvectors = pca.eigenvectors.clone();

	// Project training matrix into PCA space
	cv::Mat training_mat_pca;
	pca.project(training_mat, training_mat_pca);

	// train the SVM
	svm->train(training_mat_pca, labels_mat, cv::Mat(), cv::Mat(), params); // training
}

/* ************************************************************************* */
void saveSVM(const cv::Ptr<CvSVM> svm, const std::string &write_path)
{
	svm->save(write_path.c_str()); // saving
}
