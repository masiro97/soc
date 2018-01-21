#include "stdafx.h"

#include <algorithm>
#include "BlauFunction.h"
#include "ImageSrc\ImageFrameWndManager.h"
#include <windows.h>
#include <afxwin.h>
#define delay_max 10

using namespace std;

planar_pattern_detector *detector1, *detector2, *detector3, *detector4;
template_matching_based_tracker *tracker1, *tracker2, *tracker3, *tracker4;


int dectMP1 = 300, dectMP2 = 1000, dectMP3 = 1000, dectMP4 = 1000;

int qN = 0;
int queue_i = 0;
queue select_queue[4];


planar_pattern_detector * just_load(const char *image_name)
{
	planar_pattern_detector * detector = new planar_pattern_detector();

	char keypoint_filename[1000];
	sprintf_s(keypoint_filename, "%s.keypoint", image_name);

	ifstream keyf(keypoint_filename);

	if (keyf.good()) detector->kload(keypoint_filename);

	char detector_data_filename[1000];
	sprintf_s(detector_data_filename, "%s.detector_data", image_name);

	if (detector->load(detector_data_filename)) {
		cout << "> [planar_pattern_detector] " << detector_data_filename << " file read." << endl;
		return detector;
	}
	else {
		cerr << ">! [planar_pattern_detector] Couldn't find file " << detector_data_filename << "." << endl;
		return 0;
	}
}


int detector_Init(string model_image1, string model_image2 = "0", string model_image3 = "0", string model_image4 = "0")
{
	detector1 = just_load(model_image1.c_str());

	if (!detector1) {
		cerr << "Unable to build detector1.\n";
		return -1;
	}
	detector1->set_maximum_number_of_points_to_detect(dectMP1);

	if (model_image2 != "0") {
		detector2 = just_load(model_image2.c_str());

		if (!detector2) {
			cerr << "Unable to build detector2.\n";
			return -1;
		}
		detector2->set_maximum_number_of_points_to_detect(dectMP2);
	}

	if (model_image3 != "0") {
		detector3 = just_load(model_image3.c_str());

		if (!detector3) {
			cerr << "Unable to build detector3.\n";
			return -1;
		}
		detector3->set_maximum_number_of_points_to_detect(dectMP3);
	}

	if (model_image4 != "0") {
		detector4 = just_load(model_image4.c_str());

		if (!detector4) {
			cerr << "Unable to build detector4.\n";
			return -1;
		}
		detector4->set_maximum_number_of_points_to_detect(dectMP4);
	}
}
void tracker_Init(string model_image1, string model_image2 = "0", string model_image3 = "0", string model_image4 = "0")
{
	tracker1 = new template_matching_based_tracker();
	string trackerfn = model_image1 + string(".tracker_data");
	if (!tracker1->load(trackerfn.c_str())) cout <<"Couldn't find Tracker file of model1"<< endl;
	tracker1->initialize();

	if (model_image2 != "0") {
		tracker2 = new template_matching_based_tracker();
		trackerfn = model_image2 + string(".tracker_data");
		if (!tracker2->load(trackerfn.c_str()))  cout << "Couldn't find Tracker file of model2" << endl;
		tracker2->initialize();
	}

	if (model_image3 != "0") {
		tracker3 = new template_matching_based_tracker();
		trackerfn = model_image3 + string(".tracker_data");
		if (!tracker3->load(trackerfn.c_str())) cout << "Couldn't find Tracker file of model3" << endl;
		tracker3->initialize();
	}

	if (model_image4 != "0") {
		tracker4 = new template_matching_based_tracker();
		trackerfn = model_image4 + string(".tracker_data");
		if (!tracker4->load(trackerfn.c_str())) cout << "Couldn't find Tracker file of model4" << endl;
		tracker4->initialize();
	}
}
void end_point()
{
	delete detector1;
	delete detector2;
	delete detector3;
	delete detector4;

	delete tracker1;
	delete tracker2;
	delete tracker3;
	delete tracker4;

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

void draw_quadrangle(ByteImage * frame,
	int u0, int v0,
	int u1, int v1,
	int u2, int v2,
	int u3, int v3,
	BlauScalar color, int thickness = 1)
{
	int x = u0 + u1 + u2 + u3 / 4;
	int y = v0 + v1 + v2 + v3 / 4;
	BlauPoint center(x, y);
	//void B_PutText(ByteImage* Img, char* text, BlauPoint point, CDC* m_pDC, int m_width, int m_height, BlauScalar RGB);

	//cvLine(frame, BlauPoint(u0, v0), BlauPoint(u1, v1), color, thickness);
	//cvLine(frame, BlauPoint(u1, v1), BlauPoint(u2, v2), color, thickness);
	//cvLine(frame, BlauPoint(u2, v2), BlauPoint(u3, v3), color, thickness);
	//cvLine(frame, BlauPoint(u3, v3), BlauPoint(u0, v0), color, thickness);
}

void draw_tracker(ByteImage *frame,
	int u0, int v0,
	int u1, int v1,
	int u2, int v2,
	int u3, int v3,
	BlauScalar color, CDC* pDC, char *text, BlauScalar m_RGB) {

	int x = (u0 + u1 + u2 + u3) / 4;
	int y = (v0 + v1 + v2 + v3) / 4;
	B_PutText(frame, text, BlauPoint(x,y),pDC, 50, 30, BlauScalar(m_RGB.R,m_RGB.G,m_RGB.B));
}
void draw_initial_rectangle(ByteImage * frame, template_matching_based_tracker * tracker)
{
	draw_quadrangle(frame,
		tracker->u0[0], tracker->u0[1],
		tracker->u0[2], tracker->u0[3],
		tracker->u0[4], tracker->u0[5],
		tracker->u0[6], tracker->u0[7],
		BlauScalar(128), 3);
}

void draw_tracked_position(ByteImage * frame, template_matching_based_tracker * tracker, char * text)
{	//±Û¾¾ ¶ç¿ì±â
	/*draw_quadrangle(frame,
		tracker->u[0], tracker->u[1],
		tracker->u[2], tracker->u[3],
		tracker->u[4], tracker->u[5],
		tracker->u[6], tracker->u[7],
		BlauScalar(255), 3);*/
	CDC *PDC = AfxGetMainWnd()->GetDC();
	draw_tracker(frame,
		tracker->u[0], tracker->u[1],
		tracker->u[2], tracker->u[3],
		tracker->u[4], tracker->u[5],
		tracker->u[6], tracker->u[7],
		BlauScalar(255, 255, 255), PDC, text, BlauScalar(255, 255, 255));
	AfxGetMainWnd()->ReleaseDC(PDC);


}

void draw_tracked_locations(ByteImage * frame, template_matching_based_tracker * tracker)
{
	for (int i = 0; i < tracker->nx * tracker->ny; i++) {
		int x1, y1;
		tracker->f.transform_point(tracker->m[2 * i], tracker->m[2 * i + 1], x1, y1);
		BlauCircle(frame, BlauPoint(x1, y1), 3, BlauScalar(255, 255, 255));
	}
}

void draw_detected_position(ByteImage * frame, planar_pattern_detector * detector,char *text)
{	//±Û¾¾ ¶ç¿ì±â
	CDC *pDC = AfxGetMainWnd()->GetDC();
	B_PutText(frame, text, BlauPoint(10, 30), pDC, 50, 30, BlauScalar(255, 255, 255));
	AfxGetMainWnd()->ReleaseDC(pDC);
}

void draw_detected_keypoints(ByteImage * frame, planar_pattern_detector * detector)
{
	for (int i = 0; i < detector->number_of_detected_points; i++)
		BlauCircle(frame,
			BlauPoint(detector->detected_points[i].fr_u(),
				detector->detected_points[i].fr_v()),
			16 * (1 << int(detector->detected_points[i].scale)),
			BlauScalar(100));
}

void draw_recognized_keypoints(ByteImage * frame, planar_pattern_detector * detector)
{
	for (int i = 0; i < detector->number_of_model_points; i++)
		if (detector->model_points[i].class_score > 0)
			BlauCircle(frame,
				BlauPoint(detector->model_points[i].potential_correspondent->fr_u(),
					detector->model_points[i].potential_correspondent->fr_v()),
				16 * (1 << int(detector->detected_points[i].scale)),
				BlauScalar(255, 255, 255));
}


void detect_and_draw(ByteImage* frame, bool show_tracked_locations, bool show_keypoints)
{
	CDC *pDC = AfxGetMainWnd()->GetDC();
	static bool last_frame_ok = false;
	static queue* select_index = select_queue;
	static int frame_delay = -1;

	if (!last_frame_ok)
	{
		detector1->pyramid->set_image(frame);
		detector1->detect_points(detector1->pyramid);
	}
	//detector1->save_image_of_model_points(frame,detector1->patch_size);
	/**/
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

					if (!ok) return detect_and_draw(frame, show_tracked_locations, show_keypoints);

					else {
						draw_tracked_position(frame, tracker1, "model1");
						if (show_tracked_locations) draw_tracked_locations(frame, tracker1);
					}
					//cvPutText(frame, "template-based 3D tracking", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
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
							//draw_tracked_position(frame, tracker1);
							draw_tracked_position(frame, tracker1, "model1");
							if (show_tracked_locations) draw_tracked_locations(frame, tracker1);

							//cvPutText(frame, "detection+template-based 3D tracking", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
						}
						else {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector1);
								draw_recognized_keypoints(frame, detector1);
							}
							draw_detected_position(frame, detector1,"model1");
							//cvPutText(frame, "detection", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
						}
					}
					else {
						last_frame_ok = false;
						if (frame_delay > -1) frame_delay--;

						if (show_keypoints) draw_detected_keypoints(frame, detector1);
						//cvPutText(frame, "detection", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
					}
				}
				break;

			case 2://detector2
				if (last_frame_ok) {
					bool ok = tracker2->track(frame);
					last_frame_ok = ok;

					if (!ok) return detect_and_draw(frame, show_tracked_locations, show_keypoints);

					else {
						draw_tracked_position(frame, tracker2, "model2");
						if (show_tracked_locations) draw_tracked_locations(frame, tracker2);
					}
					//cvPutText(frame, "template-based 3D tracking", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
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
						
							draw_tracked_position(frame, tracker2, "model2");
							if (show_tracked_locations) draw_tracked_locations(frame, tracker2);

							//cvPutText(frame, "detection+template-based 3D tracking", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
						}
						else {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector2);
								draw_recognized_keypoints(frame, detector2);
							}
							draw_detected_position(frame, detector2,"model2");
							//cvPutText(frame, "detection", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
						}
					}
					else {
						last_frame_ok = false; 
						if (frame_delay > -1) frame_delay--;
						if (show_keypoints) draw_detected_keypoints(frame, detector2);

						//cvPutText(frame, "detection", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
					}
				}
				break;

			case 3://detector3
				if (last_frame_ok) {
					bool ok = tracker3->track(frame);
					last_frame_ok = ok;

					if (!ok) return detect_and_draw(frame, show_tracked_locations, show_keypoints);

					else {
				
						draw_tracked_position(frame, tracker3, "model3");
						if (show_tracked_locations) draw_tracked_locations(frame, tracker3);
					}
					//cvPutText(frame, "template-based 3D tracking", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
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
							draw_tracked_position(frame, tracker3, "model3");
							if (show_tracked_locations) draw_tracked_locations(frame, tracker3);

							//cvPutText(frame, "detection+template-based 3D tracking", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
						}
						else {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector3);
								draw_recognized_keypoints(frame, detector3);
							}
							draw_detected_position(frame, detector3,"model3");
							//cvPutText(frame, "detection", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
						}
					}
					else {
						last_frame_ok = false; 
						if (frame_delay > -1) frame_delay--;
						if (show_keypoints) draw_detected_keypoints(frame, detector3);

						//cvPutText(frame, "detection", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
					}
				}
				break;

			case 4://detector4
				if (last_frame_ok) {
					bool ok = tracker2->track(frame);
					last_frame_ok = ok;

					if (!ok) return detect_and_draw(frame, show_tracked_locations, show_keypoints);

					else {
						
					
						if (show_tracked_locations) draw_tracked_locations(frame, tracker2);
					}
					//cvPutText(frame, "template-based 3D tracking", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
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
							draw_tracked_position(frame, tracker4, "model4");
							if (show_tracked_locations) draw_tracked_locations(frame, tracker4);

							//cvPutText(frame, "detection+template-based 3D tracking", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
						}
						else {
							if (show_keypoints) {
								draw_detected_keypoints(frame, detector4);
								draw_recognized_keypoints(frame, detector4);
							}
							draw_detected_position(frame, detector4,"model4");
						//	cvPutText(frame, "detection", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
						}
					}
					else {
						last_frame_ok = false; 
						if (frame_delay > -1) frame_delay--;
						if (show_keypoints) draw_detected_keypoints(frame, detector4);

						//cvPutText(frame, "detection", BlauPoint(10, 30), &font, BlauScalar(255, 255, 255));
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
	} while (frame_delay < 0);
	/**/
	AfxGetMainWnd()->ReleaseDC(pDC);
	ShowImage(*frame, "BLAUMIND");

}