#include "stdafx.h"

#include <iostream>
#include <string>
using namespace std;

#include "ferns\planar_pattern_detector.h"
#include "ferns\template_matching_based_tracker.h"


planar_pattern_detector *detector1, *detector2, *detector3, *detector4;
template_matching_based_tracker *tracker1, *tracker2, *tracker3, *tracker4;

int dectMP1 = 300, dectMP2 = 1000, dectMP3 = 1000, dectMP4 = 1000;


planar_pattern_detector * just_load(const char *image_name)
{
	planar_pattern_detector * detector = new planar_pattern_detector();

	char keypoint_filename[1000];
	sprintf(keypoint_filename, "%s.keypoint", image_name);

	ifstream keyf(keypoint_filename);

	if (keyf.good()) detector->kload(keypoint_filename);

	char detector_data_filename[1000];
	sprintf(detector_data_filename, "%s.detector_data", image_name);

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
	if (!tracker1->load(trackerfn.c_str())) 
		
	tracker1->initialize();

	if (model_image2 != "0") {
		tracker2 = new template_matching_based_tracker();
		trackerfn = model_image2 + string(".tracker_data");
		if (!tracker2->load(trackerfn.c_str())) 
		
		tracker2->initialize();
	}

	if (model_image3 != "0") {
		tracker3 = new template_matching_based_tracker();
		trackerfn = model_image3 + string(".tracker_data");
		if (!tracker3->load(trackerfn.c_str())) 
		
		tracker3->initialize();
	}

	if (model_image4 != "0") {
		tracker4 = new template_matching_based_tracker();
		trackerfn = model_image4 + string(".tracker_data");
		if (!tracker4->load(trackerfn.c_str())) 
		
		tracker4->initialize();
	}
}
