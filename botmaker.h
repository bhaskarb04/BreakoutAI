#pragma once
#include "recorder.h"

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>

class botmaker
{
	cv::Ptr<cv::ml::RTrees> tree;
public:
	botmaker();
	~botmaker();
	static botmaker* instance();
	void train();
	void predict(info_frame& frame);
};

