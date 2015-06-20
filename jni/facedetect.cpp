/*#include "utils1.hpp"

//--------------------------------
// 전역 변수
Rect btn0, btn1;
bool isSaved = false;
bool isChanged = false;

//--------------------------------
// 마우스 이벤트 콜백 함수
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
	// 마우스 콜백 함수 등록
	namedWindow("얼굴 검출");
	setMouseCallback("얼굴 검출", onMouse);

	//------------------------------
	// 변수 설정
	int h_cnt = 0, f_cnt = -1;
	string image_url = "c:/Users/태흥/Pictures/face/";
	string msg, fn;

	btn0 = Rect(Point(10, 10), Size(140, 40));
	btn1 = Rect(Point(10, 50), Size(140, 40));

	//------------------------------
	// 검출기 읽기
	CascadeClassifier faceCascade, eyeCascade;
	load_classifier(faceCascade, faceCascadeFilename);
	load_classifier(eyeCascade, eyeCascadeFilename);

	//------------------------------
	// 카메라 초기화
	//------------------------------
	VideoCapture capture = init_camera(640, 480);

	//------------------------------
	//실시간 얼굴 검출
	//------------------------------
	while (1){
		Mat frame = get_videoframe(capture);
		Mat copy = frame.clone();

		//------------------------------
		//버튼 만들기
		Mat tmp = frame(btn0);
		tmp += (isSaved) ? Scalar(50, 50, 50) : Scalar(0, 0, 0);
		tmp = frame(btn1);
		tmp += (isChanged) ? Scalar(50, 50, 50) : Scalar(0, 0, 0);

		putText(frame, "save face", btn0.tl() + Point(5, 26), FONT_HERSHEY_DUPLEX, 0.6f, Scalar(0, 255, 0),1,8);
		rectangle(frame, btn0, Scalar(100, 100, 100));
		putText(frame, "change face", btn1.tl() + Point(5, 26), FONT_HERSHEY_DUPLEX, 0.6f, Scalar(0, 255, 0),1,8);
		rectangle(frame, btn1, Scalar(100, 100, 100));
		
		//------------------------------
		//change 버튼 토글
		if (isChanged) {
			h_cnt++; f_cnt = -1;
			isChanged = false;
			isSaved = false;
		}

		//------------------------------
		// 얼굴과 눈 검출
		Rect face_rects[3];
		Point obj_pt[3];
		Mat detectedFace = detect_object(frame, face_rects, obj_pt, faceCascade, eyeCascade);
		//------------------------------
		// 얼굴 및 눈 영역 표시
		if (obj_pt[1].x > 0 && obj_pt[2].x > 0)
		{
			draw_face_eyes(frame, face_rects, obj_pt);

			//------------------------------
			//영상 저장 - save 버튼 토글
			if (isSaved) {
				fn = format("face_%02d_%02d.png", h_cnt, ++f_cnt);
				imwrite(image_url + fn, copy);
			}
		}
		
		//------------------------------
		// 저장 정보 표시
		if (f_cnt >= 0)
			putText(frame, fn + " is saved.", Point(20, 460), FONT_HERSHEY_DUPLEX, 0.6f, Scalar(0, 255, 0),1,8);

		imshow("얼굴 검출", frame);

		if (waitKey(100) == 27) break; //실행 중지 키(esc키) 설정
	}
}*/