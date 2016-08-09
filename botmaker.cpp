#include "botmaker.h"



botmaker::botmaker()
{
}


botmaker::~botmaker()
{
}

botmaker* botmaker::instance(){
	static botmaker* _instance = nullptr;
	if (_instance == nullptr)
		_instance = new botmaker;
	return _instance;
}

void botmaker::train(){
	tree = cv::ml::RTrees::create();
	tree->train(cv::ml::TrainData::loadFromCSV("Breakoutplay.csv",0));
}

void botmaker::predict(info_frame& frame){
	cv::Mat m;
	m.push_back(frame.ballposx);
	m.push_back(frame.ballposy);
	m.push_back(frame.mvballx);
	m.push_back(frame.mvbally);
	m.push_back(frame.paddleposx);
	m.push_back(frame.paddleposy);

	int response = tree->predict(m);
	frame.keystroke = response;
}
