#include "utils1.hpp"
#include "utils2.hpp"

void main(int argc, char* argv[])
{
	//-------------------------
	// 변수 설정
	const int BORDER = 8; //최외각 테두리 크기
	const int train_class_cnt = 4; // 학습할 사람수
	const int train_face_cnt = 5; // 각 사람마다 학습할 얼굴 개수
	const Size n_face(70, 70); // 검출 얼굴의 정규 영상

	vector<int> latestface; //학습한 얼굴들의 대표얼굴 - 최종얼굴의 번호
	vector<int> faceLabels; // 각 얼굴의 인식 값
	vector<Mat> detectedFaces; // 검출 얼굴 데이터

	//-------------------------
	// 검출기 읽기
	CascadeClassifier faceCascade, eyeCascade;
	load_classifier(faceCascade, faceCascadeFilename);
	load_classifier(eyeCascade, eyeCascadeFilename);

	//-------------------------
	// 학습 얼굴 수집
	//-------------------------
	for (int i = 0; i < train_class_cnt; i++) {
		latestface.push_back(-1);
		for (int j = 0; j < train_face_cnt; j++) {

			//-------------------------
			// 학습 얼굴영상 읽기
			char fname[50];
			sprintf(fname, "face_%02d_%02d.png", i, j);
			Mat src = readImage(fname);

			//-------------------------
			// 얼굴과 눈 검출
			Rect rects[3];
			Point obj_pts[3];
			Mat face_tmp = detect_object(src, rects, obj_pts, faceCascade, eyeCascade);
			//-------------------------
			//검출 얼굴 회전 보정
			Mat corrected_face = rotated_face(face_tmp, obj_pts, n_face);

			//-------------------------
			//검출 얼굴 저장
			if (corrected_face.data) {
				Mat mirroredFace; // 좌우 반전 영상 추가
				flip(corrected_face, mirroredFace, 1);
				latestface[i] = detectedFaces.size() - 1;

				detectedFaces.push_back(corrected_face);
				detectedFaces.push_back(mirroredFace);
				faceLabels.push_back(i);
				faceLabels.push_back(i);
			}

		}
	}

	//-------------------------
	// 수집 얼굴 학습
	//-------------------------
	bool haveContribModul = initModule_contrib();
	if (!haveContribModul) { // 예외 처리
		cerr << "ERROR: The 'contrib' module is needed for ";
		cerr << "FaceRecognizer but hasn't been loaded to OpenCV!" << endl;
		exit(1);
	}

	//-------------------------
	// FaceRecognizer 객체 생성
	Ptr<FaceRecognizer> model = Algorithm::create<FaceRecognizer>(facerecAlgorithm);
	if (model.empty()) {
		cerr << "ERROR: The FaceRecognizer algorithm [" << facerecAlgorithm;
		cerr << "] is not available in your version of OpenCV.";
		cerr << "Please update to OpenCV v2.4.1 or newer." << endl;
		exit(-1);
	}
	model->train(detectedFaces, faceLabels);

	//-------------------------
	// 카메라 초기화
	//-------------------------
	//VideoCapture capture = init_camera(640, 480);
	String img_name = argv[1];



	//-------------------------
	// 실시간 얼굴 인식
	//-------------------------
	//while (1){
		//Mat frame = get_videoframe(capture); //카메라 영상 입력
		Mat frame = imread(img_name);

		//-------------------------
		// 기존 학습얼굴 대표 영상 우측 상단 표시
		Point gui_faces = Point(frame.cols - n_face.width - BORDER, BORDER);

		for (int i = 0; i < (int)latestface.size(); i++) {
			int index = latestface[i];
			if (index >= 0 && index < (int)detectedFaces.size()) {
				Mat srcGray = detectedFaces[index];
				if (srcGray.data) {
					Mat srcBGR;
					cvtColor(srcGray, srcBGR, CV_GRAY2BGR);

					int y = min(gui_faces.y + i * n_face.height, frame.rows - n_face.height);
					Rect dstRC = Rect(Point(gui_faces.x, y), n_face);
					Mat dstROI = frame(dstRC);
					srcBGR.copyTo(dstROI);
				}
			}
		}

		//-------------------------
		//얼굴과 눈 검출
		Rect rects[3];
		Point obj_pts[3];
		Mat face_tmp = detect_object(frame, rects, obj_pts, faceCascade, eyeCascade);

		//-------------------------
		// 검출 얼굴 회전 보정
		Mat corrected_face = rotated_face(face_tmp, obj_pts, n_face);

		int identity = -1;
		if (corrected_face.data) // 얼굴이 검출되면
		{
			//-------------------------
			//얼굴 및 눈 표시
			draw_face_eyes(frame, rects, obj_pts);

			//-------------------------
			//검출얼굴 중앙 상단 표시
			display_topface(frame, corrected_face, n_face, BORDER);

			//-------------------------
			//유사도 계산
			Mat reconstructedFace = reconstructFace(model, corrected_face); // 얼굴 재구성
			double similarity = getSimilarity(corrected_face, reconstructedFace); //유사도 계산

			//-------------------------
			// 얼굴 인식
			if (similarity < 1.0f) {
				identity = model->predict(corrected_face); //얼굴예측
				cout << "얼굴번호 : " << identity << ". 유사도 : " << similarity << endl;

				//-------------------------
				//인식 얼굴 우측 표시
				if (identity >= 0) {
					int y = min(gui_faces.y + identity * n_face.height, frame.rows - n_face.height);
					Rect rc = Rect(Point(gui_faces.x, y), n_face);
					rectangle(frame, rc, CV_RGB(0, 255, 0), 3, CV_AA);
				}
			}
			else {
				cout << "얼굴번호 :: Unknown. 유사도 : " << similarity << endl;
			}
		}
		imshow("얼굴 검출", frame);

		waitKey();
		//if (waitKey(100) == 27) break; //실행 중지 키(esc키) 설정
	//}
}