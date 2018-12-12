
//Llibreries declarades

//OpenCV
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

//std
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    	VideoCapture camera; //OpenCV video capture object
    	Mat image; //OpenCV image object
	int cam_id; //camera id . Associated to device number in /dev/videoX
    	Mat gray_image;
	
	// Programa per detectar cara
        CascadeClassifier face_cascade;	
	face_cascade.load("haarcascade_frontalface_default.xml");
	// Programa per detectar ulls
        CascadeClassifier eyes_cascade;	
	eyes_cascade.load("haarcascade_eye_tree_eyeglasses.xml");
	// Programa per detectar el nas
        //CascadeClassifier nose_cascade;	
	//nose_cascade.load("haarcascade_mcs_nose.xml");

	// Llegint la imatge mustache
	//Mat mustache = imread("/home/pau/Escritorio/Master en Robotica/Perception System/RoboticaUVic-FaceDetect/img/moustache.png");

	
	// Per saber si carrega be el arxiu xml o no
 		if (!face_cascade.load("haarcascade_frontalface_default.xml")) {
         		printf("--(!)Error loading the haarcascade_frontalface_default.xml\n"); return -1; 
    			}

		if (!eyes_cascade.load("haarcascade_eye_tree_eyeglasses.xml")) {
         		printf("--(!)Error loading the haarcascade_eye_tree_eyeglasses.xml\n"); return -1; 
    			}

		/*if (!nose_cascade.load("haarcascade_mcs_nose.xml")) {
         		printf("--(!)Error loading the haarcascade_mcs_nose.xml\n"); return -1; 
    			}*/

	//check user args
	switch(argc)
	{
		case 1: //no argument provided, so try /dev/video0
			cam_id = 0;
			break;
		case 2: //an argument is provided. Get it and set cam_id
			cam_id = atoi(argv[1]);
			break;
		default:
			std::cout << "Invalid number of arguments. Call program as: webcam_capture [video_device_id]. " << std::endl;
			std::cout << "EXIT program." << std::endl;
			break;
	}

	//advertising to the user
	std::cout << "Opening video device " << cam_id << std::endl;

    //open the video stream and make sure it's opened
    if( !camera.open(cam_id) )
	{
        std::cout << "Error opening the camera. May be invalid device id. EXIT program." << std::endl;
        return -1;
    }

    //Process loop. Capture and point feature extraction. User can quit pressing a key
    while(1)
	{
		//Read image and check it. Blocking call up to a new image arrives from camera.
        if(!camera.read(image))
		{
            std::cout << "No image" << std::endl;
            cv::waitKey();
        }

    //**************** Find faces in the image ****************************
  
        // If input image is RGB, convert it to gray
        cvtColor(image, gray_image, CV_BGR2GRAY);
	
	//Processem la imatge
	equalizeHist(gray_image, gray_image);

	vector<Rect> faces;
	face_cascade.detectMultiScale(gray_image, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30,30));

    for (int i = 0; i < faces.size(); i++) {
        // visualize the faces
       	Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        ellipse( image, center, Size( faces[i].width*0.5, faces[i].height*0.5), 1.1, 1, 360, Scalar( 255, 0, 255 ), 5, 15, 0 );
           
    //********************************************************************

    //**************** Find eyes in the image ****************************

	// Comentem el programa de detecció de ulls perque la camera comença anar retardada (Suposo que degut al bucle que fem per trobar la cara i els ulls)
	/*
	vector<Rect> eyes;
	eyes_cascade.detectMultiScale(gray_image, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30,30));

    		for (int j = 0; j < eyes.size(); j++) {
       		// visualize the eyes
       	    	Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       		int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
       		circle(image, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
     }
     
	*/
    }
    //********************************************************************

        //show image
        cv::imshow("Output Window", image);

		//Waits 1 millisecond to check if a key has been pressed. If so, breaks the loop. Otherwise continues.
        if((unsigned char)(cv::waitKey(1)) == 'q') break;
    }
return 0;
}
