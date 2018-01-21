/*
  Copyright 2007, 2008 Computer Vision Lab,
  Ecole Polytechnique Federale de Lausanne (EPFL), Switzerland.
  All rights reserved.

  Authors: Vincent Lepetit (http://cvlab.epfl.ch/~lepetit)
           Mustafa Ozuysal (http://cvlab.epfl.ch/~oezuysal)
           Julien  Pilet   (http://cvlab.epfl.ch/~jpilet)

  This file is part of the ferns_demo software.

  ferns_demo is free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation; either version 2 of the License, or (at your option) any later
  version.

  ferns_demo is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
  PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  ferns_demo; if not, write to the Free Software Foundation, Inc., 51 Franklin
  Street, Fifth Floor, Boston, MA 02110-1301, USA
*/
//#include <cv.h>
//#include <highgui.h>
#include "opencv2/highgui/highgui.hpp"


#include <iostream>
#include <string>
using namespace std;

#include "mcv.h"
#include "planar_pattern_detector_builder.h"
#include "template_matching_based_tracker.h"

const int max_filename = 1000;

enum source_type {webcam_source, sequence_source, video_source};

planar_pattern_detector * detector;
template_matching_based_tracker * tracker;

int mode = 2;
bool show_tracked_locations = true;
bool show_keypoints = true;

CvFont font;

void draw_quadrangle(IplImage * frame,
		     int u0, int v0,
		     int u1, int v1,
		     int u2, int v2,
		     int u3, int v3,
		     CvScalar color, int thickness = 1)
{
  cvLine(frame, cvPoint(u0, v0), cvPoint(u1, v1), color, thickness);
  cvLine(frame, cvPoint(u1, v1), cvPoint(u2, v2), color, thickness);
  cvLine(frame, cvPoint(u2, v2), cvPoint(u3, v3), color, thickness);
  cvLine(frame, cvPoint(u3, v3), cvPoint(u0, v0), color, thickness);
}

void draw_detected_position(IplImage * frame, planar_pattern_detector * detector)
{
	cvPutText(frame, "Chilsung Cider", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
  /*draw_quadrangle(frame,
		  detector->detected_u_corner[0], detector->detected_v_corner[0],
		  detector->detected_u_corner[1], detector->detected_v_corner[1],
		  detector->detected_u_corner[2], detector->detected_v_corner[2],
		  detector->detected_u_corner[3], detector->detected_v_corner[3],
		  cvScalar(255), 3);*/
}

void draw_initial_rectangle(IplImage * frame, template_matching_based_tracker * tracker)
{
  draw_quadrangle(frame,
		  tracker->u0[0], tracker->u0[1],
		  tracker->u0[2], tracker->u0[3],
		  tracker->u0[4], tracker->u0[5],
		  tracker->u0[6], tracker->u0[7],
		  cvScalar(128), 3);
}

void draw_tracked_position(IplImage * frame, template_matching_based_tracker * tracker)
{
 draw_quadrangle(frame,
		  tracker->u[0], tracker->u[1],
		  tracker->u[2], tracker->u[3],
		  tracker->u[4], tracker->u[5],
		  tracker->u[6], tracker->u[7],
		  cvScalar(255), 3);

	cvPutText(frame, "Chilsung Cider", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
}

void draw_tracked_locations(IplImage * frame, template_matching_based_tracker * tracker)
{
  for(int i = 0; i < tracker->nx * tracker->ny; i++) {
    int x1, y1;
    tracker->f.transform_point(tracker->m[2 * i], tracker->m[2 * i + 1], x1, y1);
    //cvCircle(frame, cvPoint(x1, y1), 3, cvScalar(255, 255, 255), 1);
  }
}

void draw_detected_keypoints(IplImage * frame, planar_pattern_detector * detector)
{
 for(int i = 0; i < detector->number_of_detected_points; i++)
    cvCircle(frame,
	     cvPoint(detector->detected_points[i].fr_u(),
		     detector->detected_points[i].fr_v()),
	     16 * (1 << int(detector->detected_points[i].scale)),
	     cvScalar(100), 1);
}

void draw_recognized_keypoints(IplImage * frame, planar_pattern_detector * detector)
{
  for(int i = 0; i < detector->number_of_model_points; i++)
    if (detector->model_points[i].class_score > 0)
      cvCircle(frame,
	       cvPoint(detector->model_points[i].potential_correspondent->fr_u(),
		       detector->model_points[i].potential_correspondent->fr_v()),
	       16 * (1 << int(detector->detected_points[i].scale)),
	       cvScalar(255, 255, 255), 1);
}


void detect_and_draw(IplImage * frame)
{
	static bool last_frame_ok=false;

	if (mode == 1 || ((mode==0) && last_frame_ok)) {
		bool ok = tracker->track(frame);
		last_frame_ok=ok;


		if (!ok) {
			if (mode==0) return detect_and_draw(frame);
			else {
				draw_initial_rectangle(frame, tracker);
				tracker->initialize();
			}
		} else {
			draw_tracked_position(frame, tracker);
			if (show_tracked_locations) draw_tracked_locations(frame, tracker);
		}
		//cvPutText(frame, "template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
	} else {
		detector->detect(frame);

		if (detector->pattern_is_detected) {
			last_frame_ok=true;

			tracker->initialize(detector->detected_u_corner[0], detector->detected_v_corner[0],
					detector->detected_u_corner[1], detector->detected_v_corner[1],
					detector->detected_u_corner[2], detector->detected_v_corner[2],
					detector->detected_u_corner[3], detector->detected_v_corner[3]);

			if (mode == 3 && tracker->track(frame)) {

				if (show_keypoints) {
					draw_detected_keypoints(frame, detector);
					draw_recognized_keypoints(frame, detector);
				}
				draw_tracked_position(frame, tracker);
				if (show_tracked_locations) draw_tracked_locations(frame, tracker);

				//cvPutText(frame, "detection+template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
			} else {
				if (show_keypoints) {
					draw_detected_keypoints(frame, detector);
					draw_recognized_keypoints(frame, detector);
				}
				draw_detected_position(frame, detector);
				//cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
			}
		} else {
			last_frame_ok=false;
			if (show_keypoints) draw_detected_keypoints(frame, detector);
			/*
			if (mode == 3)
				cvPutText(frame, "detection + template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
			else
				cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));*/
		}
	}

	cvShowImage("ferns-demo", frame);
}

void help(const string& exec_name) {
  cout << exec_name << " [-m <model image>] [-s <image sequence format>]\n\n";
  cout << "   -m : specify the name of the model image depicting the planar \n";
  cout << "        object from a frontal viewpoint. Default model.bmp\n";
  cout << "   -s : image sequence format in printf style, e.g. image%04.jpg,\n";
  cout << "        to test detection. If not specified webcam is used as \n";
  cout << "        image source.\n";
  cout << "   -v : video filename to test detection. If not specified webcam\n";
  cout << "        is used as image source.\n";
  cout << "   -h : This help message." << endl;
}




int main(int argc, char ** argv)
{
	string model_image = "pepsi18.bmp";
  string sequence_format = "";
  string video_file = "mousepad.mp4";
  source_type frame_source = webcam_source;
  //source_type frame_source = video_source;

  for(int i = 0; i < argc; ++i) {
    if(strcmp(argv[i], "-h") == 0) {
      help(argv[0]);
      return 0;
    }

    if(strcmp(argv[i], "-m") == 0) {
      if(i == argc - 1) {
        cerr << "Missing model name after -m\n";
        help(argv[0]);
        return -1;
      }
      ++i;
      model_image = argv[i];
    }
    else if(strcmp(argv[i], "-s") == 0) {
      if(i == argc - 1) {
        cerr << "Missing sequence format after -s\n";
        help(argv[0]);
        return -1;
      }
      ++i;
      sequence_format = argv[i];
      frame_source = sequence_source;
    }
    else if(strcmp(argv[i], "-v") == 0) {
      if(i == argc - 1) {
        cerr << "Missing  video filename after -v\n";
        help(argv[0]);
        return -1;
      }
      ++i;
      video_file = argv[i];
      frame_source = video_source;
    }
  }

  affine_transformation_range range;

  detector = planar_pattern_detector_builder::build_with_cache(model_image.c_str(),
							       &range,
							       100,
							       5000,
							       0.0,
							       32, 7, 5,
							       30, 15,
							       100000, 200);

  if (!detector) {
    cerr << "Unable to build detector.\n";
    return -1;
  }

  detector->set_maximum_number_of_points_to_detect(300);

  tracker = new template_matching_based_tracker();
  string trackerfn = model_image + string(".tracker_data");
  if (!tracker->load(trackerfn.c_str())) {
    cout << "Training template matching..."<<endl;
    tracker->learn(detector->model_image,
		   6, // number of used matrices (coarse-to-fine)
		   40, // max motion in pixel used to train to coarser matrix
		   10, 10, // defines a grid. Each cell will have one tracked point.
		   detector->u_corner[0], detector->v_corner[1],
		   detector->u_corner[2], detector->v_corner[2],
		   40, 40, // neighbordhood for local maxima selection
		   10000 // number of training samples
		   );
    tracker->save(trackerfn.c_str());
  }
  tracker->initialize();

  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX,
	     1.0, 1.0, 0.0,
	     3, 8);

  CvCapture * capture = 0;
  IplImage * frame, * gray_frame = 0;
  int frame_id = 1;
  char seq_buffer[max_filename];

  cvNamedWindow("ferns-demo", 1 );

  if(frame_source == webcam_source) {
    capture = cvCaptureFromCAM(1);
  }
  else if(frame_source == video_source) {
    capture = cvCreateFileCapture(video_file.c_str());
  }

  int threshold = 200;
  int64 timer = cvGetTickCount();

  bool stop = false;
  do {
    if(frame_source == webcam_source || frame_source == video_source) {
      if (cvGrabFrame(capture) == 0) break;
      frame = cvRetrieveFrame(capture);
    }
    else {
//      snprintf(seq_buffer, max_filename, sequence_format.c_str(), frame_id);
      _snprintf(seq_buffer, max_filename, sequence_format.c_str(), frame_id);
      frame = cvLoadImage(seq_buffer, 1);
      ++frame_id;
    }
	
    if (frame == 0) break;

    if (gray_frame == 0)
     gray_frame = cvCreateImage(cvSize(frame->width,frame->height), IPL_DEPTH_8U, 1);
	//IplImage *output = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
	cvCvtColor(frame, gray_frame, CV_RGB2GRAY);
	//cvThreshold(gray_frame, output, threshold, 255, CV_THRESH_BINARY);
   // cvCvtColor(frame, gray_frame,CV_RGB2);
	//threshold(frame, gray_frame, 100, 255, THRESH_OTSU);
    if (frame->origin != IPL_ORIGIN_TL)
      cvFlip(gray_frame, gray_frame, 0);
	
   detect_and_draw(gray_frame);

    int64 now = cvGetTickCount();
    double fps = 1e6 * cvGetTickFrequency()/double(now-timer);
    timer = now;
    clog << "Detection frame rate: " << fps << " fps         \r";

    int key = cvWaitKey(10);
    if (key >= 0) {
      switch(char(key)) {
      case '0': mode = 0; break;
      case '1': mode = 1; break;
      case '2': mode = 2; break;
      case '3': mode = 3; break;
      case '4': show_tracked_locations = !show_tracked_locations; break;
      case '5': show_keypoints = !show_keypoints; break;
      case 'q': stop = true; break;
      default: ;
      }
      cout << "mode=" << mode << endl;
    }

    if(frame_source == sequence_source) {
      cvReleaseImage(&frame);
    }
  } while(!stop);

  clog << endl;
  delete detector;
  delete tracker;

  cvReleaseImage(&gray_frame);
  cvReleaseCapture(&capture);
  cvDestroyWindow("ferns-demo");

  return 0;
}



/*

#include <stdio.h>

#include <vector>




//OpenCV 관련 헤더 인클루드

#include <opencv\cv.h>       

#include <opencv\highgui.h>  




//contrast stretching 할 때 로워바운드 어퍼바운드 비율 

#define LOWER_PERCENT 0.05

#define UPPER_PERCENT 0.95


void main()

{

IplImage* image = 0;                           //구조체(?) 이 포인터로 이미지를 받고 이 포인터를 이용해서 이미지를 처리함

CvCapture* capture = cvCaptureFromCAM(0);              //카메라로부터 컨트롤(?)을 얻어옴. (0번 카메라연결?)

cvNamedWindow("rabbit", 0);                 //윈도우 생성. 숫자는 윈도우의 크기를 나타냄 0:고정된 윈도우, 1:자동 조절 윈도우

cvResizeWindow("rabbit", 320, 240);        //윈도우의 크기를 조정할 수 있음


while (1) {                                       //카메라로 부터 매 프레임을 받아오기 위해서 무한루프를 돌린다.

cvGrabFrame(capture);                  //입력된 영상의 한 프레임을 잡고,
image = cvRetrieveFrame(capture);    //잡은 프레임으로부터 이미지를 생성
IplImage *gray_frame = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
IplImage *gray_frame2 = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
cvCvtColor(image,gray_frame, CV_RGB2GRAY);

HistogramEqualization(gray_frame, *gray_frame2);
//cvShowImage("rabbit", gray_frame2);        //생성된 이미지를 윈도우에 보여줌
if (cvWaitKey(10) >= 0)

break;

}

cvReleaseCapture(&capture);             //카메라와의 연결 종료

cvDestroyWindow("히스토그램 평활화 결과 영상");               //윈도우 종료

}
*/
/*
void main()

{

	 

	//이미지를 띄울 창을 생성

	cvNamedWindow("Source Image", CV_WINDOW_AUTOSIZE);

	
	//히스토그램 draw

	IplImage *hist_view;

	IplImage *hist_stretched_view;

	IplImage *stretched;

	CvCapture* capture = cvCaptureFromCAM(1);
	IplImage *image = 0;
	while (1)

	{
		cvGrabFrame(capture);                  //입력된 영상의 한 프레임을 잡고,
		image = cvRetrieveFrame(capture);    //잡은 프레임으로부터 이미지를 생성
		hist_view = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
		hist_stretched_view = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
		stretched = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
		cvCvtColor(image, hist_view, CV_RGB2GRAY);
		cvCvtColor(image, hist_stretched_view, CV_RGB2GRAY);
		cvCvtColor(image, stretched, CV_RGB2GRAY);

		//constrast stretching

		//히스토그램 생성

		long hist_arr[256] = { 0 };

		for (int i = 0; i<image->height; ++i)

		{

			for (int j = 0; j<image->width; ++j)

			{

				hist_arr[(int)cvGetReal2D(stretched, i, j)]++;

			}

		}


		///////////////////////////////////////////////////////////////////////////////

		//히스토그램 cdf 만들기

		long hist_cdf[256] = { 0 };  //누적함수

		hist_cdf[0] = hist_arr[0];

		for (int i = 1; i<256; ++i)

			hist_cdf[i] = hist_arr[i] + hist_cdf[i - 1];

		///////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////

		//최대, 최소 threhold 구하기

		int lower_bound = 0;

		int upper_bound = 0;

		double num_px = image->width*image->height;


		for (int i = 0; i<256; ++i) //lower bound

		{

			if ((double)hist_cdf[i] / num_px > LOWER_PERCENT)

			{

				lower_bound = i;

				break;

			}

		}

		for (int i = 255; i >= 0; --i) //upper bound

		{

			if ((double)hist_cdf[i] / num_px < UPPER_PERCENT)

			{

				upper_bound = i;

				break;

			}

		}

		printf("uppper bound :%d, lower bound :%d\n", upper_bound, lower_bound);

		///////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////

		//contrast stretching 수행 (normalization)

		for (int i = 0; i<image->height; ++i)

		{

			for (int j = 0; j<image->width; ++j)

			{

				int temp_val;

				temp_val = (double)(cvGetReal2D(stretched, i, j) - lower_bound) / (double)(upper_bound - lower_bound) * 255.0;

				if (temp_val < 0)

					temp_val = 0;

				if (temp_val > 255)

					temp_val = 255;

				cvSetReal2D(stretched, i, j, temp_val);

			}

		}

		///////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////

		//히스토그램 그리기

		{

			cvZero(hist_view);

			//우선 히스토그램 max값을 알아낸다.  normalize 위해서

			int max_val = 0;

			for (int i = 0; i<255; ++i)

			{

				if (hist_arr[i] > max_val)

					max_val = hist_arr[i];

			}

			//cvShowImage("Histogram", stretched);

		}

		//연산된 이미지의 히스토그램 얻고 그리기

		//히스토그램 생성

		{

			long hist_streched_arr[256] = { 0 };

			for (int i = 0; i<image->height; ++i)

			{

				for (int j = 0; j<image->width; ++j)

				{

					hist_streched_arr[(int)cvGetReal2D(stretched, i, j)]++;

				}

			}
			//히스토그램 그리기

			cvZero(hist_stretched_view);

			//우선 히스토그램 max값을 알아낸다.  normalize 위해서

			int max_val = 0;

			for (int i = 0; i<255; ++i)

			{

				if (hist_streched_arr[i] > max_val)

					max_val = hist_streched_arr[i];

			}

		}


		//cvShowImage("Histogram", hist_view);
		//cvShowImage("Stretched Histogram", hist_stretched_view);

		///////////////////////////////////////////////////////////////////////////////

//		cvShowImage("Source Image", src);

		//cvShowImage("Source Gray", src_grey);

		cvShowImage("Stretched", stretched);




		int key_input = cvWaitKey(20);




		//key input

		if (key_input == 'q')  //quit

			break;

	}




	//생성한 창 닫기		

	cvDestroyAllWindows();

	if (stretched)

		cvReleaseImage(&stretched);

	return;

}

*/
