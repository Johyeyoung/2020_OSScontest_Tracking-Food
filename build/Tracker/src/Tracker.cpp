///////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2020 KOBOT kobot9@naver.com.
// All rights reserved.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE
//
//////////////////////////////////////////////////////////////////////////////////

#include "Tracker.h"


Point2i pt;


Tracker::Tracker(){}


//젓가락 좌표 반환하는 함수
Vec4i Tracker::detect_chopstic(Mat img, int thr, int minLineLength, int maxLineGap)
{
	Vec4i cur_line;
		lines.clear();
		HoughLinesP(img, lines, 1, CV_PI / 180, thr, minLineLength, maxLineGap);
		if (lines.empty())
		{
			return NULL;
		}
		for (int i = 0; i < lines.size(); i++)
		{
			cur_line += lines[i];
		}
		int size = lines.size();
		cur_line = cur_line / size;
		//cout << (last_line[1] > last_line[3] ? pow(last_line[2] - cur_line[2], 2) + pow(last_line[3] - cur_line[3], 2) : pow(last_line[0] - cur_line[0], 2) + pow(last_line[1] - cur_line[1], 2)) << endl;
		int error = (last_line[1] > last_line[3] ? pow(last_line[2] - cur_line[2], 2) + pow(last_line[3] - cur_line[3], 2) : pow(last_line[0] - cur_line[0], 2) + pow(last_line[1] - cur_line[1], 2));
		last_line = cur_line;
		if (error > 10000) cur_line = last_line;
		return cur_line;
}


//숟가락 좌표 반환하는 함수
Vec3f Tracker::detect_spoon(Mat img, int minDistance, int thr)
{
	circles.clear();
	HoughCircles(img, circles, HOUGH_GRADIENT, 1, minDistance, thr, thr / 2, 10, 70);
	if (circles.empty())
	{
		return NULL;
	}

	return circles[0];
}


//살색 지우는 함수
Mat Tracker::make_mask_image(Mat img)
{
	cvtColor(img, img_YCrCb, COLOR_BGR2YCrCb);
	inRange(img_YCrCb, Scalar(0, 133, 77), Scalar(255, 173, 127), mask);
	bitwise_not(mask, mask);
	return mask;
}


//tracking point 젓가락의 좌표 반환
void Tracker::track_point()
{	
	VideoCapture cap(-1);

	cap.set(3, 640);
	cap.set(4, 480);
	Mat frame;
	while(1){
		cap >> frame;

		if (frame.empty()){
			cout << "Could not open camera :(" << endl;
		}

		//frame 영상 전처리(640x480, 좌우반전, 3x3 가우시안 블러)
		flip(frame, frame, 1);
		GaussianBlur(frame, blur, Size(5, 5), 0);

		//초기 차영상 마스크 Mat 생성
		if (foreground_mask.empty()){
			foreground_mask.create(frame.size(), frame.type());
		}

		// 1.차영상 마스크 씌우고 노이즈 제거(9x9 미디언 필터)
		bg_model->apply(blur, foreground_mask,0);
		Mat kernel = getStructuringElement(MORPH_RECT, Size(9, 9), Point(-1, -1));
		morphologyEx(foreground_mask, foreground_mask, MORPH_CLOSE, kernel);
		medianBlur(foreground_mask, foreground_mask, 5);


		// 2.차영상에서 살색제거 후 skin_mask에 저장
		Mat copy_frame = frame.clone();

		skin_mask = make_mask_image(frame);

		// 3.식기만 남은 영상 이진화
		bitwise_and(skin_mask, foreground_mask, foreground_mask);

		// 4.이진화 영상에 Canny 적용
		Canny(foreground_mask, img_canny, 50, 150);
		
		/*
		// 5.숟가락을 찾으면 바로 반환하고, 숟가락이 없으면 젓가락 좌표 반환
		//숟가락 포인트
		Vec3f s = NULL;
		s = detect_spoon(img_canny, 10, 50);
		if (s[0] > 0 && s[1] > 0 && s[2] > 0){
			circle(frame, Point(s[0], s[1]), s[2], Scalar(0, 255, 0), 2);
			circle(frame, Point(s[0], s[1]), 2, Scalar(255, 0, 0), -1);
			//디버깅용
			//imshow("canny_img", frame);
			cout << Point(s[0], s[1]);
			//waitKey();
			pt = Point(s[0], s[1]);
		}
		*/ 
		//젓가락 포인트
		Vec4i c = NULL;
		c = detect_chopstic(img_canny, 50, 50, 100);
		if (c[0] > 0 && c[1] > 0 && c[2] > 0 && c[3] > 0){
			line(frame, Point(c[0], c[1]), Point(c[2], c[3]), Scalar(0, 255, 0), 2, 10);
			
			// circle(frame,Point(c[0], c[1]), 5, Scalar(0, 0, 255), -1);
			//디버깅용
			//imshow("canny_img", frame);
			//cout << Point(c[1] > c[3] ? c[2] : c[0], c[1] > c[3] ? c[3] : c[1])<<endl;
			//waitKey();

			//영상에서 더 위에있는 포인트 반환
			pt = Point(c[3] > c[1] ? c[2] : c[0], c[3] > c[1] ? c[3] : c[1]);
			// circle(frame,Point(c[3] > c[1] ? c[2] : c[0], c[3] > c[1] ? c[3] : c[1]), 5, Scalar(255, 0, 0), -1);
			circle(frame,Point(pt.x,pt.y), 5, Scalar(255, 0, 0), -1);	
		}

		imshow("test", frame);
		imshow("test2", img_canny);
		//디버깅용
		if(waitKey(1) == 27){
			cap.release();
			destroyAllWindows();
			break;
		}

		//cout << Point(c[1] > c[3] ? c[2] : c[0], c[1] > c[3] ? c[3] : c[1])<<endl;
		//waitKey();
	}
}
int Tracker::get_track_point_x(){
	return pt.x;
}
int Tracker::get_track_point_y(){
	return pt.y;
}


extern "C" {

	Tracker* Tracker_new()
	{
		return new Tracker();
	}
	//젓가락 좌표 반환하는 함수
	Vec4i detect_chopstic(Tracker * t, Mat img, int thr, int minLineLength, int maxLineGap){return t->detect_chopstic(img, thr, minLineLength, maxLineGap);}
	//숟가락 좌표 반환하는 함수
	Vec3f detect_spoon(Tracker * t, Mat img, int minDistance, int thr){return t->detect_spoon(img, minDistance, thr);}
	//살색 지우는 함수
	Mat make_mask_image(Tracker * t, Mat img){return t-> make_mask_image(img);}
	//tracking point 젓가락의 좌표 반환 _______ 이것만 사용!!
	void track_point(Tracker * t){t->track_point();}

	int track_point_x(Tracker * t){return t->get_track_point_x();}
	int track_point_y(Tracker * t){return t->get_track_point_y();}
}
