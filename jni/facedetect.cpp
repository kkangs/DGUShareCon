/*#include "utils1.hpp"

//--------------------------------
// ���� ����
Rect btn0, btn1;
bool isSaved = false;
bool isChanged = false;

//--------------------------------
// ���콺 �̺�Ʈ �ݹ� �Լ�
void onMouse(int event, int x, int y, int flags, void*)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		if (btn0.contains(Point(x,y))) isSaved = !isSaved;
		if (btn1.contains(Point(x,y))) isChanged = true;
	}
}

void main()
{
	//------------------------------
	// ���콺 �ݹ� �Լ� ���
	namedWindow("�� ����");
	setMouseCallback("�� ����", onMouse);

	//------------------------------
	// ���� ����
	int h_cnt = 0, f_cnt = -1;
	string image_url = "c:/Users/����/Pictures/face/";
	string msg, fn;

	btn0 = Rect(Point(10, 10), Size(140, 40));
	btn1 = Rect(Point(10, 50), Size(140, 40));

	//------------------------------
	// ����� �б�
	CascadeClassifier faceCascade, eyeCascade;
	load_classifier(faceCascade, faceCascadeFilename);
	load_classifier(eyeCascade, eyeCascadeFilename);

	//------------------------------
	// ī�޶� �ʱ�ȭ
	//------------------------------
	VideoCapture capture = init_camera(640, 480);

	//------------------------------
	//�ǽð� �� ����
	//------------------------------
	while (1){
		Mat frame = get_videoframe(capture);
		Mat copy = frame.clone();

		//------------------------------
		//��ư �����
		Mat tmp = frame(btn0);
		tmp += (isSaved) ? Scalar(50, 50, 50) : Scalar(0, 0, 0);
		tmp = frame(btn1);
		tmp += (isChanged) ? Scalar(50, 50, 50) : Scalar(0, 0, 0);

		putText(frame, "save face", btn0.tl() + Point(5, 26), FONT_HERSHEY_DUPLEX, 0.6f, Scalar(0, 255, 0),1,8);
		rectangle(frame, btn0, Scalar(100, 100, 100));
		putText(frame, "change face", btn1.tl() + Point(5, 26), FONT_HERSHEY_DUPLEX, 0.6f, Scalar(0, 255, 0),1,8);
		rectangle(frame, btn1, Scalar(100, 100, 100));
		
		//------------------------------
		//change ��ư ���
		if (isChanged) {
			h_cnt++; f_cnt = -1;
			isChanged = false;
			isSaved = false;
		}

		//------------------------------
		// �󱼰� �� ����
		Rect face_rects[3];
		Point obj_pt[3];
		Mat detectedFace = detect_object(frame, face_rects, obj_pt, faceCascade, eyeCascade);
		//------------------------------
		// �� �� �� ���� ǥ��
		if (obj_pt[1].x > 0 && obj_pt[2].x > 0)
		{
			draw_face_eyes(frame, face_rects, obj_pt);

			//------------------------------
			//���� ���� - save ��ư ���
			if (isSaved) {
				fn = format("face_%02d_%02d.png", h_cnt, ++f_cnt);
				imwrite(image_url + fn, copy);
			}
		}
		
		//------------------------------
		// ���� ���� ǥ��
		if (f_cnt >= 0)
			putText(frame, fn + " is saved.", Point(20, 460), FONT_HERSHEY_DUPLEX, 0.6f, Scalar(0, 255, 0),1,8);

		imshow("�� ����", frame);

		if (waitKey(100) == 27) break; //���� ���� Ű(escŰ) ����
	}
}*/