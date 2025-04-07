#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------

 //Below is my code for my original project that I realized would unfortunately not work
 //It was meant to be an attendance checker for zoom that would match the names from the class roster to those listed in the zoom participant list
 //Unfortunately, I realized there would be many factors that wouldn't allow this to work, such as different sized fonts, nicknames, etc.

//Mat  imgCanny, imgDil, imgErode;
//
//void getContours(Mat imgDil, Mat img) {
//	vector<vector<Point>> contours;
//	vector<Vec4i> hierarchy;
//	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//
//	for (int i = 0; i < contours.size(); i++) {
//		int area = contourArea(contours[i]);
//		cout << area << endl;
//		vector<vector<Point>> conPoly(contours.size());
//		vector<Rect> boundRect(contours.size());
//		if (area != 1120) {
//			if (area != 560) {
//
//				approxPolyDP(contours[i], conPoly[i], 3, true);
//				drawContours(img, conPoly, i, Scalar(255, 0, 255), 1);
//				/*cout << conPoly[i].size() << endl;*/
//				boundRect[i] = boundingRect(conPoly[i]);
//				rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
//			}
//		}
//	}
//}
////First, you must upload a file containing screenshots of the first zoom meeting (full attendance) and a screenshot of the zoom class for the day you want to take attendance to the project file folder.
//void main() {
//	/*string i;
//	cout << "Enter the name of the original class roster file as formatted here (foldername/filename)." << endl;
//	cin >> i;
//	string path = i;*/
//	string path = "OpenCVTestStuff/namestest.png";
//	Mat img = imread(path);
//	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
//	Canny(img, imgCanny, 10, 25);
//	dilate(imgCanny, imgDil, kernel);
//	getContours(imgDil,img);
//	/*CascadeClassifier nameCascade;
//	nameCascade.load("OpenCVTestStuff/haarcascade_russian_plate_number.xml");
//	vector<Rect> names;
//	
//		nameCascade.detectMultiScale(img, names, 1.3, 1);
//		for (int x = 0; x < names.size(); x++) {
//			rectangle(img, names[x].tl(), names[x].br(), Scalar(255, 0, 255), 3);
//		}*/
//	imshow("Image", img);
//	
//
//
//	waitKey(0);
//	
//}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------

// First, add a folder with (an) image(s) of shapes into the project folder
// The shapes code only works with the listed shapes: Circle, Square, Rectangle, Triangle, Pentagon, Hexagon. Any other shapes will either misidentify or if there are more than 6 sides will identify as a circle.


Mat imgGray, imgBlur, imgCanny, imgDil;

//The general idea for this getContours function was found online, though I made many edits to it so it would fit my needs for the project.
void getContours(Mat imgDil, Mat img) {
	vector<vector<Point>> contours;
	//string ask = "What shape is this?";     Part of the failed interactable implementation as mentioned below
	vector<Vec4i> hierarchy;
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//int a = contours.size();  Part of the failed interactable implementation as mentioned below
	for (int i = 0; i < contours.size(); i++) {
		
		int area = contourArea(contours[i]);
		//cout << area << endl;   Used to print out the area of the shapes, to help create a filter to ignore potential "noise" in an image, as seen with the shapes.png image
		vector<vector<Point>> conPoly(contours.size());
		string objectType;
		vector<Rect> boundRect(contours.size());
		if (area > 1000) {

			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			//cout << conPoly[i].size() << endl;     Used to print out the number of points in the shapes
			boundRect[i] = boundingRect(conPoly[i]);


			int objCor = (int)conPoly[i].size();
			if (objCor == 3) { objectType = "Triangle"; }
			if (objCor == 4) {
			float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
			if (aspRatio > 0.95 && aspRatio < 1.05) { objectType = "Square"; }
			else { objectType = "Rectangle"; }
		}
			if (objCor == 5) { objectType = "Pentagon"; }
			if (objCor == 6) { objectType = "Hexagon"; }
			if (objCor > 6) { objectType = "Circle"; }

			/* Made attempts to make this program interactable, such that the shapes would be numbered and you could type in the names 
			* of the shapes and if correct it would move on, but putting a cin into this function made the whole code not work, couldn't figure out why...
			* Also, depending on the orientation of the shapes in whatever image used, the numbering would occasionally come out in a very odd order.
			
			stringstream ss;
			ss << i;
			string str = ss.str(); 
			string shape;
			cin >> shape;
			if (shape==objectType){
			cout << "Correct!" << endl;

			*/
			putText(img, objectType, {boundRect[i].x + 15,boundRect[i].y + 50}, FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 0), 1.5);

			
			
		}

	}
}

void main() {
	string pic;
	cout << "Enter the file name you want to use as shown. folder/filename.filetype: ";
	cin >> pic;
	string path = pic;

	Mat imgComp = imread(path);
	Mat img = imread(path);

	//The 5 lines below are a sort of preprocessing method for the image provided, so as to give the program clearer and more precise lines to follow.
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	/* Original idea for text, made the image look too crowded.
	
	string og = "Press any key to see";
	string og1 = "shape names";

		putText(imgComp, og, { 5,50 }, FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 0), 1.5);
		putText(imgComp, og1, { 10,75 }, FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 0), 1.5);*/
		imshow("Shapes", imgComp);

		cout << "Try to name the shapes yourself, then press any button to see the answer key." << endl;
		waitKey(0);
		getContours(imgDil, img);
		imshow("Shapes", img);

		waitKey(0);


}