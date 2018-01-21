#pragma once
#include <iostream>
#include <string>

#include "ferns\planar_pattern_detector.h"
#include "ferns\template_matching_based_tracker.h"

class queue { public: int index; int score = 0; };

planar_pattern_detector * just_load(const char *image_name);
int detector_Init(string model_image1, string model_image2 , string model_image3 , string model_image4 );
void tracker_Init(string model_image1, string model_image2 , string model_image3 , string model_image4 );
void end_point();

void queue_Init(string model_image1, string model_image2 , string model_image3 , string model_image4 );
static bool cmp_nodes(const queue & k1, const queue & k2);
void Update_queue();
queue* queue_out();

void draw_quadrangle(ByteImage * frame,
	int u0, int v0,
	int u1, int v1,
	int u2, int v2,
	int u3, int v3,
	BlauScalar color, int thickness);
void draw_tracker(ByteImage *frame,
	int u0, int v0,
	int u1, int v1,
	int u2, int v2,
	int u3, int v3,
	BlauScalar color, CDC* pDC, char *text, BlauScalar m_RGB);
void draw_initial_rectangle(ByteImage * frame, template_matching_based_tracker * tracker);
void draw_tracked_position(ByteImage * frame, template_matching_based_tracker * tracker, char * text);
void draw_tracked_locations(ByteImage * frame, template_matching_based_tracker * tracker);
void draw_detected_position(ByteImage * frame, planar_pattern_detector * detector, char *text);
void draw_detected_keypoints(ByteImage * frame, planar_pattern_detector * detector);
void draw_recognized_keypoints(ByteImage * frame, planar_pattern_detector * detector);
void detect_and_draw(ByteImage* frame, bool show_tracked_locations, bool show_keypoints);