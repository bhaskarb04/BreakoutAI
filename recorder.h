#pragma once
#include <iostream>
#include <fstream>
using namespace std;
struct info_frame{
	double paddleposx, paddleposy;
	double ballposx, ballposy;
	double mvballx, mvbally;
	int keystroke;
};
class recorder
{
	fstream *fp;
public:
	static recorder* instance();
	recorder();
	~recorder();
	void record(info_frame frame);
};

