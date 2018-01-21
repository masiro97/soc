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
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <string>
using namespace std;

#include "mcv.h"
#include "planar_pattern_detector_builder.h"
#include "template_matching_based_tracker.h"

#define delay_max 10

const int max_filename = 1000;

planar_pattern_detector *detector1, *detector2, *detector3, *detector4;
template_matching_based_tracker *tracker1, *tracker2, *tracker3, *tracker4;

class queue { public: int index; int score=0; };

bool show_tracked_locations = true;
bool show_keypoints = true;

CvFont font;

int qN=0;
int queue_i = 0;
queue select_queue[4];


affine_transformation_range range;
int dectMP1 = 300, dectMP2 = 1000, dectMP3 = 1000, dectMP4 = 1000;

//init
int detector_Init(string model_image1, string model_image2="0", string model_image3 = "0", string model_image4 = "0")
{
	detector1 = planar_pattern_detector_builder::build_with_cache(model_image1.c_str(),
		&range,
		10,
		100,
		0.0,
		16, 7, 4,
		30, 15,
		150, 500);

	if (!detector1) {
		cerr << "Unable to build detector1.\n";
		return -1;
	}
	detector1->set_maximum_number_of_points_to_detect(dectMP1);

	if (model_image2 != "0") {
		detector2 = planar_pattern_detector_builder::build_with_cache(model_image2.c_str(),
			&range,
			200,
			1000,
			0.0,
			32, 7, 4,
			30, 8,
			5000, 500);

		if (!detector2) {
			cerr << "Unable to build detector2.\n";
			return -1;
		}
		detector2->set_maximum_number_of_points_to_detect(dectMP2);
	}

	if (model_image3 != "0") {
		detector3 = planar_pattern_detector_builder::build_with_cache(model_image3.c_str(),
			&range,
			200,
			1000,
			0.0,
			32, 7, 4,
			30, 8,
			5000, 500);

		if (!detector3) {
			cerr << "Unable to build detector2.\n";
			return -1;
		}
		detector3->set_maximum_number_of_points_to_detect(dectMP3);
	}

	if (model_image4 != "0") {
		detector4 = planar_pattern_detector_builder::build_with_cache(model_image4.c_str(),
			&range,
			200,
			1000,
			0.0,
			32, 7, 4,
			30, 8,
			5000, 500);

		if (!detector4) {
			cerr << "Unable to build detector2.\n";
			return -1;
		}
		detector4->set_maximum_number_of_points_to_detect(dectMP4);
	}
}
void tracker_Init(string model_image1, string model_image2 = "0", string model_image3 = "0", string model_image4 = "0")
{
	tracker1 = new template_matching_based_tracker();
	string trackerfn = model_image1 + string(".tracker_data");
	if (!tracker1->load(trackerfn.c_str())) {
		cout << "Training template matching(model_1)..." << endl;
		tracker1->learn(detector1->model_image,
			5, // number of used matrices (coarse-to-fine)
			40, // max motion in pixel used to train to coarser matrix
			20, 20, // defines a grid. Each cell will have one tracked point.
			detector1->u_corner[0], detector1->v_corner[1],
			detector1->u_corner[2], detector1->v_corner[2],
			40, 40, // neighbordhood for local maxima selection
			100 // number of training samples
			);
		tracker1->save(trackerfn.c_str());
	}
	tracker1->initialize();

	if (model_image2 != "0") {
		tracker2 = new template_matching_based_tracker();
		trackerfn = model_image2 + string(".tracker_data");
		if (!tracker2->load(trackerfn.c_str())) {
			cout << "Training template matching(model_2)..." << endl;
			tracker2->learn(detector2->model_image,
				6, // number of used matrices (coarse-to-fine)
				40, // max motion in pixel used to train to coarser matrix
				30, 30, // defines a grid. Each cell will have one tracked point.
				detector2->u_corner[0], detector2->v_corner[1],
				detector2->u_corner[2], detector2->v_corner[2],
				40, 40, // neighbordhood for local maxima selection
				10000 // number of training samples
				);
			tracker2->save(trackerfn.c_str());
		}
		tracker2->initialize();
	}

	if (model_image3 != "0") {
		tracker3 = new template_matching_based_tracker();
		trackerfn = model_image3 + string(".tracker_data");
		if (!tracker3->load(trackerfn.c_str())) {
			cout << "Training template matching(model_3)..." << endl;
			tracker3->learn(detector3->model_image,
				5, // number of used matrices (coarse-to-fine)
				40, // max motion in pixel used to train to coarser matrix
				20, 20, // defines a grid. Each cell will have one tracked point.
				detector3->u_corner[0], detector3->v_corner[1],
				detector3->u_corner[2], detector3->v_corner[2],
				40, 40, // neighbordhood for local maxima selection
				10000 // number of training samples
				);
			tracker3->save(trackerfn.c_str());
		}
		tracker3->initialize();
	}

	if (model_image4 != "0") {
		tracker4 = new template_matching_based_tracker();
		trackerfn = model_image4 + string(".tracker_data");
		if (!tracker4->load(trackerfn.c_str())) {
			cout << "Training template matching(model_4)..." << endl;
			tracker4->learn(detector4->model_image,
				5, // number of used matrices (coarse-to-fine)
				40, // max motion in pixel used to train to coarser matrix
				20, 20, // defines a grid. Each cell will have one tracked point.
				detector4->u_corner[0], detector4->v_corner[1],
				detector4->u_corner[2], detector4->v_corner[2],
				40, 40, // neighbordhood for local maxima selection
				10000 // number of training samples
				);
			tracker4->save(trackerfn.c_str());
		}
		tracker4->initialize();
	}
}

void queue_Init(string model_image1, string model_image2 = "0", string model_image3 = "0", string model_image4 = "0")
{
	select_queue[0].index = 1; qN++;
	if (model_image2 != "0") { select_queue[qN].index = 2; qN++; }
	if (model_image3 != "0") { select_queue[qN].index = 3; qN++; }
	if (model_image4 != "0") { select_queue[qN].index = 4; qN++; }
}
static bool cmp_nodes(const queue & k1, const queue & k2)
{
	return k1.score < k2.score;
}
void Update_queue()
{
	sort(select_queue, select_queue + qN, cmp_nodes);
	for (int i = 0; i < qN; i++) select_queue[i].score = 0;
}
queue* queue_out()
{
	if (queue_i == qN) {
		queue_i = 0; return NULL;
	}
	else { queue_i++; return &select_queue[queue_i - 1]; }
}


//drawing
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
}

void draw_tracked_locations(IplImage * frame, template_matching_based_tracker * tracker)
{
	for (int i = 0; i < tracker->nx * tracker->ny; i++) {
		int x1, y1;
		tracker->f.transform_point(tracker->m[2 * i], tracker->m[2 * i + 1], x1, y1);
		cvCircle(frame, cvPoint(x1, y1), 3, cvScalar(255, 255, 255), 1);
	}
}

void draw_detected_position(IplImage * frame, planar_pattern_detector * detector)
{
	draw_quadrangle(frame,
		detector->detected_u_corner[0], detector->detected_v_corner[0],
		detector->detected_u_corner[1], detector->detected_v_corner[1],
		detector->detected_u_corner[2], detector->detected_v_corner[2],
		detector->detected_u_corner[3], detector->detected_v_corner[3],
		cvScalar(255), 3);
}

void draw_detected_keypoints(IplImage * frame, planar_pattern_detector * detector)
{
	for (int i = 0; i < detector->number_of_detected_points; i++)
		cvCircle(frame,
			cvPoint(detector->detected_points[i].fr_u(),
				detector->detected_points[i].fr_v()),
			16 * (1 << int(detector->detected_points[i].scale)),
			cvScalar(100), 1);
}

void draw_recognized_keypoints(IplImage * frame, planar_pattern_detector * detector)
{
	for (int i = 0; i < detector->number_of_model_points; i++)
		if (detector->model_points[i].class_score > 0)
			cvCircle(frame,
				cvPoint(detector->model_points[i].potential_correspondent->fr_u(),
					detector->model_points[i].potential_correspondent->fr_v()),
				16 * (1 << int(detector->detected_points[i].scale)),
				cvScalar(255, 255, 255), 1);
}


void detect_and_draw(IplImage * frame)
{	
	static bool last_frame_ok = false;
	static queue* select_index = select_queue;
	static int frame_delay = -1;
	
	detector1->pyramid->set_image(frame);
	detector1->detect_points(detector1->pyramid);

	do {
		if (frame_delay < 0) select_index = queue_out();

		if (select_index != NULL)
		{
			switch (select_index->index)
			{
			case 1://detector1
				if (last_frame_ok) {
					bool ok = tracker1->track(frame);
					last_frame_ok = ok;

					if (!ok) return detect_and_draw(frame);

					else {
						draw_tracked_position(frame, tracker1);
						if (show_tracked_locations) draw_tracked_locations(frame, tracker1);
					}
					cvPutText(frame, "template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
				}
				else {
					detector1->detect(frame);

					if (detector1->pattern_is_detected) {
						last_frame_ok = true; frame_delay = delay_max; 
						
						tracker1->initialize(detector1->detected_u_corner[0], detector1->detected_v_corner[0],
							detector1->detected_u_corner[1], detector1->detected_v_corner[1],
							detector1->detected_u_corner[2], detector1->detected_v_corner[2],
							detector1->detected_u_corner[3], detector1->detected_v_corner[3]);

						if (tracker1->track(frame)) {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector1);
								draw_recognized_keypoints(frame, detector1);
							}
							draw_tracked_position(frame, tracker1);
							if (show_tracked_locations) draw_tracked_locations(frame, tracker1);

							cvPutText(frame, "detection+template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
						}
						else {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector1);
								draw_recognized_keypoints(frame, detector1);
							}
							draw_detected_position(frame, detector1);
							cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
						}
					}
					else {
						last_frame_ok = false;
						if (frame_delay > -1) frame_delay--;
						
						if (show_keypoints) draw_detected_keypoints(frame, detector1);
						cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
					}
				}
				break;

			case 2://detector2
				if (last_frame_ok) {
					bool ok = tracker2->track(frame);
					last_frame_ok = ok;

					if (!ok) return detect_and_draw(frame);

					else {
						draw_tracked_position(frame, tracker2);
						if (show_tracked_locations) draw_tracked_locations(frame, tracker2);
					}
					cvPutText(frame, "template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
				}
				else {
					detector2->pyramid = detector1->pyramid;
					detector2->detected_points = detector1->detected_points;
					detector2->number_of_detected_points = detector1->number_of_detected_points;
					detector2->detect(frame);

					if (detector2->pattern_is_detected) {
						last_frame_ok = true; frame_delay = delay_max; 

						tracker2->initialize(detector2->detected_u_corner[0], detector2->detected_v_corner[0],
							detector2->detected_u_corner[1], detector2->detected_v_corner[1],
							detector2->detected_u_corner[2], detector2->detected_v_corner[2],
							detector2->detected_u_corner[3], detector2->detected_v_corner[3]);

						if (tracker2->track(frame)) {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector2);
								draw_recognized_keypoints(frame, detector2);
							}
							draw_tracked_position(frame, tracker2);
							if (show_tracked_locations) draw_tracked_locations(frame, tracker2);

							cvPutText(frame, "detection+template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
						}
						else {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector2);
								draw_recognized_keypoints(frame, detector2);
							}
							draw_detected_position(frame, detector2);
							cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
						}
					}
					else {
						last_frame_ok = false; frame_delay--;
						if (show_keypoints) draw_detected_keypoints(frame, detector2);

						cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
					}
				}
				break;

			case 3://detector3
				if (last_frame_ok) {
					bool ok = tracker3->track(frame);
					last_frame_ok = ok;

					if (!ok) return detect_and_draw(frame);

					else {
						draw_tracked_position(frame, tracker3);
						if (show_tracked_locations) draw_tracked_locations(frame, tracker3);
					}
					cvPutText(frame, "template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
				}
				else {
					detector3->pyramid = detector1->pyramid;
					detector3->detected_points = detector1->detected_points;
					detector3->number_of_detected_points = detector1->number_of_detected_points;
					detector3->detect(frame);

					if (detector3->pattern_is_detected) {
						last_frame_ok = true; frame_delay = delay_max;

						tracker3->initialize(detector3->detected_u_corner[0], detector3->detected_v_corner[0],
							detector3->detected_u_corner[1], detector3->detected_v_corner[1],
							detector3->detected_u_corner[2], detector3->detected_v_corner[2],
							detector3->detected_u_corner[3], detector3->detected_v_corner[3]);

						if (tracker3->track(frame)) {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector3);
								draw_recognized_keypoints(frame, detector3);
							}
							draw_tracked_position(frame, tracker3);
							if (show_tracked_locations) draw_tracked_locations(frame, tracker3);

							cvPutText(frame, "detection+template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
						}
						else {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector3);
								draw_recognized_keypoints(frame, detector3);
							}
							draw_detected_position(frame, detector3);
							cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
						}
					}
					else {
						last_frame_ok = false; frame_delay--;
						if (show_keypoints) draw_detected_keypoints(frame, detector3);

						cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
					}
				}
				break;

			case 4://detector4
				if (last_frame_ok) {
					bool ok = tracker2->track(frame);
					last_frame_ok = ok;

					if (!ok) return detect_and_draw(frame);

					else {
						draw_tracked_position(frame, tracker2);
						if (show_tracked_locations) draw_tracked_locations(frame, tracker2);
					}
					cvPutText(frame, "template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
				}
				else {
					detector4->pyramid = detector1->pyramid;
					detector4->detected_points = detector1->detected_points;
					detector4->number_of_detected_points = detector1->number_of_detected_points;
					detector4->detect(frame);

					if (detector4->pattern_is_detected) {
						last_frame_ok = true; frame_delay = delay_max;

						tracker4->initialize(detector4->detected_u_corner[0], detector4->detected_v_corner[0],
							detector4->detected_u_corner[1], detector4->detected_v_corner[1],
							detector4->detected_u_corner[2], detector4->detected_v_corner[2],
							detector4->detected_u_corner[3], detector4->detected_v_corner[3]);

						if (tracker4->track(frame)) {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector4);
								draw_recognized_keypoints(frame, detector4);
							}
							draw_tracked_position(frame, tracker4);
							if (show_tracked_locations) draw_tracked_locations(frame, tracker4);

							cvPutText(frame, "detection+template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
						}
						else {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector4);
								draw_recognized_keypoints(frame, detector4);
							}
							draw_detected_position(frame, detector4);
							cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
						}
					}
					else {
						last_frame_ok = false; frame_delay--;
						if (show_keypoints) draw_detected_keypoints(frame, detector4);

						cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
					}
				}
				break;
			}
			if (last_frame_ok && (select_index->score < delay_max)) select_index->score++;
		}
		else {
			Update_queue(); 
			break;
		}
	}while (frame_delay < 0);

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
  string model_image1     = "jolly.bmp";
  string model_image2	  = "0";
  string model_image3	  = "0";
  string model_image4	  = "0";

  //detector_Init
  if (detector_Init(model_image1, model_image2, model_image3, model_image4) == -1)  return -1;
  
  //tracker_Init
  tracker_Init(model_image1, model_image2, model_image3, model_image4);
 
  //queue of model Init
  queue_Init(model_image1, model_image2, model_image3, model_image4);

  //Start Cam
  cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX,
	     1.0, 1.0, 0.0,
	     3, 8);

  CvCapture * capture = 0;
  IplImage * frame, * gray_frame = 0;
  int frame_id = 1;
  char seq_buffer[max_filename];

  cvNamedWindow("ferns-demo", 1 );

  capture = cvCaptureFromCAM(1);


  int64 timer = cvGetTickCount();

  //loop
  bool stop = false;
  do {
    
    if (cvGrabFrame(capture) == 0) break;
    frame = cvRetrieveFrame(capture);

    if (frame == 0) break;

    if (gray_frame == 0)
      gray_frame = cvCreateImage(cvSize(frame->width,frame->height), IPL_DEPTH_8U, 1);

    cvCvtColor(frame, gray_frame, CV_RGB2GRAY);

    if (frame->origin != IPL_ORIGIN_TL)
      cvFlip(gray_frame, gray_frame, 1);


    detect_and_draw(gray_frame);


    int64 now = cvGetTickCount();
    double fps = 1e6 * cvGetTickFrequency()/double(now-timer);
    timer = now;
    clog << "Detection frame rate: " << fps << " fps         \r";

    int key = cvWaitKey(10);
    if (key >= 0) {
		switch (char(key)) {
		case '4': show_tracked_locations = !show_tracked_locations; break;
		case '5': show_keypoints = !show_keypoints; break;
		case 'q': stop = true; break;
		default:;
		}
    }

  } while(!stop);

  clog << endl;
  delete detector1;
  delete detector2;
  delete detector3; 
  delete detector4;

  delete tracker1;
  delete tracker2;
  delete tracker3;
  delete tracker4;

  cvReleaseImage(&gray_frame);
  cvReleaseCapture(&capture);
  cvDestroyWindow("ferns-demo");

  return 0;
}
