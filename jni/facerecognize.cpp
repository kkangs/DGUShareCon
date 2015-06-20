#include "utils1.hpp"
#include "utils2.hpp"

void main(int argc, char* argv[])
{
	//-------------------------
	// ���� ����
	const int BORDER = 8; //�ֿܰ� �׵θ� ũ��
	const int train_class_cnt = 4; // �н��� �����
	const int train_face_cnt = 5; // �� ������� �н��� �� ����
	const Size n_face(70, 70); // ���� ���� ���� ����

	vector<int> latestface; //�н��� �󱼵��� ��ǥ�� - �������� ��ȣ
	vector<int> faceLabels; // �� ���� �ν� ��
	vector<Mat> detectedFaces; // ���� �� ������

	//-------------------------
	// ����� �б�
	CascadeClassifier faceCascade, eyeCascade;
	load_classifier(faceCascade, faceCascadeFilename);
	load_classifier(eyeCascade, eyeCascadeFilename);

	//-------------------------
	// �н� �� ����
	//-------------------------
	for (int i = 0; i < train_class_cnt; i++) {
		latestface.push_back(-1);
		for (int j = 0; j < train_face_cnt; j++) {

			//-------------------------
			// �н� �󱼿��� �б�
			char fname[50];
			sprintf(fname, "face_%02d_%02d.png", i, j);
			Mat src = readImage(fname);

			//-------------------------
			// �󱼰� �� ����
			Rect rects[3];
			Point obj_pts[3];
			Mat face_tmp = detect_object(src, rects, obj_pts, faceCascade, eyeCascade);
			//-------------------------
			//���� �� ȸ�� ����
			Mat corrected_face = rotated_face(face_tmp, obj_pts, n_face);

			//-------------------------
			//���� �� ����
			if (corrected_face.data) {
				Mat mirroredFace; // �¿� ���� ���� �߰�
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
	// ���� �� �н�
	//-------------------------
	bool haveContribModul = initModule_contrib();
	if (!haveContribModul) { // ���� ó��
		cerr << "ERROR: The 'contrib' module is needed for ";
		cerr << "FaceRecognizer but hasn't been loaded to OpenCV!" << endl;
		exit(1);
	}

	//-------------------------
	// FaceRecognizer ��ü ����
	Ptr<FaceRecognizer> model = Algorithm::create<FaceRecognizer>(facerecAlgorithm);
	if (model.empty()) {
		cerr << "ERROR: The FaceRecognizer algorithm [" << facerecAlgorithm;
		cerr << "] is not available in your version of OpenCV.";
		cerr << "Please update to OpenCV v2.4.1 or newer." << endl;
		exit(-1);
	}
	model->train(detectedFaces, faceLabels);

	//-------------------------
	// ī�޶� �ʱ�ȭ
	//-------------------------
	//VideoCapture capture = init_camera(640, 480);
	String img_name = argv[1];



	//-------------------------
	// �ǽð� �� �ν�
	//-------------------------
	//while (1){
		//Mat frame = get_videoframe(capture); //ī�޶� ���� �Է�
		Mat frame = imread(img_name);

		//-------------------------
		// ���� �н��� ��ǥ ���� ���� ��� ǥ��
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
		//�󱼰� �� ����
		Rect rects[3];
		Point obj_pts[3];
		Mat face_tmp = detect_object(frame, rects, obj_pts, faceCascade, eyeCascade);

		//-------------------------
		// ���� �� ȸ�� ����
		Mat corrected_face = rotated_face(face_tmp, obj_pts, n_face);

		int identity = -1;
		if (corrected_face.data) // ���� ����Ǹ�
		{
			//-------------------------
			//�� �� �� ǥ��
			draw_face_eyes(frame, rects, obj_pts);

			//-------------------------
			//����� �߾� ��� ǥ��
			display_topface(frame, corrected_face, n_face, BORDER);

			//-------------------------
			//���絵 ���
			Mat reconstructedFace = reconstructFace(model, corrected_face); // �� �籸��
			double similarity = getSimilarity(corrected_face, reconstructedFace); //���絵 ���

			//-------------------------
			// �� �ν�
			if (similarity < 1.0f) {
				identity = model->predict(corrected_face); //�󱼿���
				cout << "�󱼹�ȣ : " << identity << ". ���絵 : " << similarity << endl;

				//-------------------------
				//�ν� �� ���� ǥ��
				if (identity >= 0) {
					int y = min(gui_faces.y + identity * n_face.height, frame.rows - n_face.height);
					Rect rc = Rect(Point(gui_faces.x, y), n_face);
					rectangle(frame, rc, CV_RGB(0, 255, 0), 3, CV_AA);
				}
			}
			else {
				cout << "�󱼹�ȣ :: Unknown. ���絵 : " << similarity << endl;
			}
		}
		imshow("�� ����", frame);

		waitKey();
		//if (waitKey(100) == 27) break; //���� ���� Ű(escŰ) ����
	//}
}