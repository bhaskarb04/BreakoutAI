#include "recorder.h"


recorder::recorder()
{
	fp = new fstream("RecordMe.csv", ios::out);
}


recorder::~recorder()
{
	fp->close();
}

recorder* recorder::instance(){
	static recorder* _instance = nullptr;
	if (_instance == nullptr)
		_instance = new recorder;
	return _instance;
}

void recorder::record(info_frame frame){
	*fp << frame.ballposx << "," << frame.ballposy << ","
		<< frame.mvballx << "," << frame.mvbally << ","
		<< frame.paddleposx << "," << frame.paddleposy << ","
		<< frame.keystroke << endl;
}