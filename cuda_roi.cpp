#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "opencv2/core/mat.hpp"
#include <cmath>
#include <ctime>
#include <algorithm>
#include <sstream>
#include "cuda_detect.h"

using namespace cv;
using namespace std;

Mat frame;
Mat Roi1;
Mat Roi2;
Mat Roi3;
Mat roi_img;
Mat imggray;
Mat canny_output;
Mat src;
Mat1b mask1;
Mat3b res;
RNG rng(12345);
Mat image_shape2 ;
Mat3b Black_img(src.rows,src.cols,Vec3b(255,255,25));
VideoCapture source=VideoCapture();
const string trackbarWindowName = "Trackbars";
char TrackbarName_r[3] = "R";
char TrackbarName_g[3] = "G";
char TrackbarName_b[3] = "B";

/* This functions does the color segmentation using the seed point. It segments the color on basis of Black, Red, White and Blue color. */

void Image_Segment_Function(int x_coord1, int y_coord1)
{
        cout<< "hello"<<endl;
	int R1= 0, B1= 0, G1 =0; 
 cout<< "hello1"<<endl;
	Mat image_shape1 = src.clone();
 cout<< "hello2"<<endl;
	Vec3b rgb1 = image_shape1.at<Vec3b>(y_coord1,x_coord1);
 cout<< "hello3"<<endl;
 	B1=rgb1.val[0];
 cout<< "hello4"<<endl;
 	G1=rgb1.val[1];
 	R1=rgb1.val[2];
	cout <<"R1 "<< R1 <<" G1 " << G1 <<" B1 " << B1 << endl;
	
	cout <<endl;	
	if(((R1 >=R_min_B) && (R1 <= R_max_B)) && ((G1 >= G_min_B) && (G1 <= G_max_B)) && ((B1 >= B_min_B) && (B1 <= B_max_B)))// BLACK
	{
		R_low1 = R_min_B, R_high1 = R_max_B, G_low1 = G_min_B, G_high1 = G_max_B, B_low1 = B_min_B, B_high1 = B_max_B;
		cout <<"BLACK"<<endl;
		flag = 1;
	}
	if(((R1 >=R_min_R) && (R1 <= R_max_R)) && ((G1 >= G_min_R) && (G1 <= G_max_R)) && ((B1 >= B_min_R) && (B1 <= B_max_R)))//RED
	{
		R_low1 = R_min_R, R_high1 = R_max_R, G_low1 = G_min_R, G_high1 = G_max_R, B_low1 = B_min_R, B_high1 = B_max_R;
		cout <<"RED"<<endl;
		flag1 = 1;
	}
	if(((R1 >=R_min_W) && (R1 <= R_max_W)) && ((G1 >= G_min_W) && (G1 <= G_max_W)) && ((B1 >= B_min_W) && (B1 <= B_max_W)))
	{
		R_low1 = R_min_W, R_high1 = R_max_W, G_low1 = G_min_W, G_high1 = G_max_W, B_low1 = B_min_W, B_high1 = B_max_W;
		cout <<"WHITE"<<endl;
		flag2 = 1;
	}
	if(((R1 >=R_min_Bl) && (R1 <= R_max_Bl)) && ((G1 >= G_min_Bl) && (G1 <= G_max_Bl)) && ((B1 >= B_min_Bl) && (B1 <= B_max_Bl)))
	{
		R_low1 = R_min_Bl, R_high1 = R_max_Bl, G_low1 = G_min_Bl, G_high1 = G_max_Bl, B_low1 = B_min_Bl, B_high1 = B_max_Bl;
		cout <<"BLUE"<<endl;
		flag3 = 1;
	}
	if(((R1 >=R_min_Y) && (R1 <= R_max_Y)) && ((G1 >= G_min_Y) && (G1 <= G_max_Y)) && ((B1 >= B_min_Y) && (B1 <= B_max_Y)))
	{
		R_low1 = R_min_Y, R_high1 = R_max_Y, G_low1 = G_min_Y, G_high1 = G_max_Y, B_low1 = B_min_Y, B_high1 = B_max_Y;
		cout <<"YELLOW"<<endl;
		flag4 = 1;
	}
else {}
	
}

/* This function creates the trackbars to track the RGB file.*/

void createTrackbars() 
{
	namedWindow(trackbarWindowName, 0);     	
        createTrackbar(TrackbarName_r, trackbarWindowName, &R_MIN, R_MAX);
	createTrackbar(TrackbarName_g, trackbarWindowName, &G_MIN, G_MAX);
	createTrackbar(TrackbarName_b, trackbarWindowName, &B_MIN, B_MAX);
}

/* This functions is optional. It is used only if the user wants to select the ROI manually.*/

void MouseCallBackCropFunc(int event, int x, int y, int flags, void* userdata)// Manual click of two points xmin, xmax, ymin, ymax
{
	static int count = 0;
	static int x_coord1, x_coord2, y_coord1, y_coord2, offset_x,offset_y;
	if  ( event == EVENT_LBUTTONDOWN )
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		if(count == 0)
		{
		  	x_coord1 = x;
                  	y_coord1 = y;
		  	count = count+1; 
 		  	line(Black_img,Point(x_coord1 + 10,y_coord1),Point(x_coord1-10,y_coord1), Scalar(0,255,0), 2, 8, 0  );
        	  	line(Black_img,Point(x_coord1,y_coord1 + 10),Point(x_coord1,y_coord1-10), Scalar(0,255,0), 2, 8, 0  );
                  	imshow("My Window", Black_img);
		}	
		else if(count == 1){
		  x_coord2 = x;
		  y_coord2 = y;
		  cout << "Left button of the mouse is clicked - position ("<< "x1 "<< x_coord1 << "y1 " << y_coord1 << ")" << endl;
		  cout << "Left button of the mouse is clicked - position (" <<"x2 " << x_coord2 << "y2 " << y_coord2 << ")" << endl;
		  line(Black_img,Point(x_coord2 + 10,y_coord2),Point(x_coord2-10,y_coord2), Scalar(0,255,0), 2, 8, 0  );
        	  line(Black_img,Point(x_coord2,y_coord2 + 10),Point(x_coord2,y_coord2-10), Scalar(0,255,0), 2, 8, 0  );
		  imshow("My Window", Black_img);
		  offset_x = (x_coord2 - x_coord1);
                  offset_y = (y_coord2 - y_coord1);	
 		  count = 0;
		  Rect Rec(x_coord1, y_coord1,offset_x , offset_y);
		  Mat Roi = Black_img(Rec);
		  imwrite("ROI.jpg",Roi);
		  namedWindow("ROI",WINDOW_NORMAL);  
                  imshow("ROI", Roi);
		}	              	
	}
}

/*This functions works with the manual ROI selection function for manually segmenting out the color by clicking on the desired color.*/

void MouseCallBackFunc(int event, int x, int y, int flags, void* userdata)// manual rgb color selection
{
	if  ( event == EVENT_LBUTTONDOWN )
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		Mat image=src.clone();
		Vec3b rgb=image.at<Vec3b>(y,x);
 		B=rgb.val[0];
 		G=rgb.val[1];
 		R=rgb.val[2];
		cout <<"R "<< R <<" G " << G <<" B " << B << endl;
		Mat HSV;
		Mat RGB=image(Rect(x,y,20,20));//capture that pixel in its own ROI
  		cvtColor(RGB, HSV,CV_BGR2HSV);
        	line(image,Point(x+20,y),Point(x-20,y), Scalar(0,255,0), 2, 8, 0  );
        	line(image,Point(x,y+20),Point(x,y-20), Scalar(0,255,0), 2, 8, 0  );
		imshow("My Window", image);
   		Vec3b hsv=HSV.at<Vec3b>(0,0);
    	 	H=hsv.val[0];
    	 	S=hsv.val[1];
    	 	V=hsv.val[2];
		setTrackbarPos(TrackbarName_r, trackbarWindowName, R);
 		setTrackbarPos(TrackbarName_g, trackbarWindowName, G);
		setTrackbarPos(TrackbarName_b, trackbarWindowName, B);
			/*int flag_1 = Image_Segment_Function(x,y);
			if(flag_1){ 
			cout << "manual" <<endl;
			inRange(src, Scalar(B_low1, G_low1, R_low1), Scalar(B_high1, G_high1, R_high1),mask1); 
                        namedWindow("mask1",WINDOW_NORMAL);
			imshow("mask1",mask1);
     		        Mat1b mask = mask1;
			res = src.clone();
			src.copyTo(Black_img,mask1);
			namedWindow("Detected",WINDOW_NORMAL);
    			imshow("Detected", Black_img);
			setMouseCallback("Detected", MouseCallBackCropFunc, 0 );
			flag_1 = 0;
			}*/
	}
}

/*This function saves the video frame and applies shape, size and color filteration to obtain the ROI automatically. The user has to only right-click on the video to save the image.*/

void onMouse(int event, int x, int y, int, void* ){ // mouse click to pause the video

if( event == EVENT_LBUTTONUP ){
		waitKey(0);
		imshow("Video", frame);
		pos =getTrackbarPos("seek", "Video");
		source.set(CAP_PROP_POS_FRAMES, pos);		
		return;
	}
	else if(event == EVENT_RBUTTONDOWN){ // right click to save the frame
		
		time_t now = time(0);
		string timeStamp=ctime(&now);
		string name = "Capture"+ timeStamp+".jpg";
		imwrite(name.c_str(),frame);
 		src = imread(name); 
		namedWindow("My Window",WINDOW_NORMAL); 
  		imshow("My Window", src);      
		cvtColor(src, imggray,CV_BGR2GRAY);
		blur(imggray, canny_output, Size(3,3));
		vector<Vec3f> circles;
		Mat image_shape = src.clone();
		Mat image_shape4 = src.clone();
		image_shape2 =  src.clone();
		/*HoughCircles(imggray, circles, HOUGH_GRADIENT, 1, 200, 100, 30, 1, 30 );
		cout <<"no of circles "<< circles.size()<<endl;	
		for( size_t i = 0; i < circles.size(); i++ )
		{
  			 Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
   			 int radius = cvRound(circles[i][2]);	
			
			if((radius >= 16)  && (radius <= 24))
			{
				//int flag2 = Image_Segment_Function(center.x, center.y);
				//if(flag2){
					//Rect r(abs(center.x - radius), abs(center.y - radius), (radius *2), (radius *2));		
					//Mat Roi = src(r);
					//ostringstream oss1;
					//oss1 << ROIcount;
					//string name2 = "ROI";
					//name2 += oss1.str() +".jpg";
					//imwrite(name2.c_str(),Roi);						
					//namedWindow("ROI",WINDOW_NORMAL);  
                			//imshow("ROI", Roi);
					// circle center
  				//	 circle( image_shape2, center, 3, Scalar(0,255,0), -1, 8, 0 );
  				 	// circle outline
   				//	circle( image_shape2, center, radius, Scalar(0,0,255), 3, 8, 0 );
					//imshow("My Window", image_shape2);
				//	ROIcount++;
				//}
			}
 		}*/
		Canny(imggray,canny_output,80,240,3);
		namedWindow("Canny",WINDOW_NORMAL);
		imshow("Canny",canny_output);
		vector<vector<Point> >contours;	
    		findContours(canny_output.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
		vector<vector<Point> >result(contours.size());
		int hexcount = 0, fourpointcontour = 0;
    		for(int i=0; i<contours.size(); ++i)
    		{
		    approxPolyDP(contours[i], result[i], arcLength(contours[i],true) * 0.02  , true);   	    
    		}		
		for(int i=0; i<result.size(); ++i)
    		{//13
			 if(result[i].size() == 4)
		    	 {//12
				fourpointcontour++;
				if((((abs(result[i][0].x -result[i][1].x) >=0) 
					&& (abs(result[i][0].x -result[i][1].x) <=25))
					&& ((abs(result[i][1].x -result[i][2].x) >=0) 
					&& (abs(result[i][1].x -result[i][2].x) <=25))
					&& ((abs(result[i][2].x -result[i][3].x) >=0) 
					&& (abs(result[i][2].x -result[i][3].x) <=25))) 
					|| (((abs(result[i][0].y -result[i][1].y) >=0) 
					&& (abs(result[i][0].y -result[i][1].y) <=25)) 
					&& ((abs(result[i][1].y -result[i][2].y) >=0) 
					&& (abs(result[i][1].y -result[i][2].y) <=25))
					&& ((abs(result[i][2].y -result[i][3].y) >=0)
					&& (abs(result[i][2].y -result[i][3].y) <=25)))) 
				{//6
				}//6
				else if(((((abs(result[i][0].x -result[i][1].x) >=0) 
					&& (abs(result[i][0].x -result[i][1].x) <=25)) 
					&& ((abs(result[i][1].x -result[i][2].x) >=0) 
					&& (abs(result[i][1].x -result[i][2].x) <=25))) 
					|| (((abs(result[i][1].x -result[i][2].x) >=0) 
					&& (abs(result[i][1].x -result[i][2].x) <=25)) 
					&& ((abs(result[i][2].x -result[i][3].x) >=0) 
					&& (abs(result[i][2].x -result[i][3].x) <=25))) 
					|| (((abs(result[i][0].x -result[i][2].x) >=0) 
					&& (abs(result[i][0].x -result[i][2].x) <=25)) 
					&& ((abs(result[i][2].x -result[i][3].x) >=0) 
					&& (abs(result[i][2].x -result[i][3].x) <=25))) 
					||(((abs(result[i][0].x -result[i][1].x) >=0) 
					&& (abs(result[i][0].x -result[i][1].x) <=25)) 
					&& ((abs(result[i][1].x -result[i][3].x) >=0) 
					&& (abs(result[i][1].x -result[i][3].x) <=25)))) 
					|| ((((abs(result[i][0].y -result[i][1].y) >=0) 
					&& (abs(result[i][0].y -result[i][1].y) <=25)) 
					&& ((abs(result[i][1].y -result[i][2].y) >=0) 
					&& (abs(result[i][1].y -result[i][2].y) <=25))) 
					|| (((abs(result[i][1].y -result[i][2].y) >=0) 
					&& (abs(result[i][1].y -result[i][2].y) <=25)) 
					&& ((abs(result[i][2].y -result[i][3].y) >=0)
					&& (abs(result[i][2].y -result[i][3].y) <=25)))
					|| (((abs(result[i][0].y -result[i][2].y) >=0) 
					&& (abs(result[i][0].y -result[i][2].y) <=25)) 
					&& ((abs(result[i][2].y -result[i][3].y) >=0) 
					&& (abs(result[i][2].y -result[i][3].y) <=25))) 
					||(((abs(result[i][0].y -result[i][1].y) >=0) 
					&& (abs(result[i][0].y -result[i][1].y) <=25))
					&& ((abs(result[i][1].y -result[i][3].y) >=0) 
					&& (abs(result[i][1].y -result[i][3].y) <=25)))))
				{//7
				}//7
				else if(((abs(result[i][0].x -result[i][1].x) >=0) 
					 && (abs(result[i][0].x -result[i][1].x) <=25)) 
					 && ((abs(result[i][0].y -result[i][1].y) >=0)  
					 && (abs(result[i][0].y -result[i][1].y) <=25)) 
					 || ((abs(result[i][1].x -result[i][2].x) >=0) 
					 && (abs(result[i][1].x -result[i][2].x) <=25)) 
					 && ((abs(result[i][1].y -result[i][2].y) >=0)  
					 && (abs(result[i][1].y -result[i][2].y) <=25)) 
					 || ((abs(result[i][2].x -result[i][3].x) >=0)  
					 && (abs(result[i][2].x -result[i][3].x) <=25)) 
					 && ((abs(result[i][2].y -result[i][3].y) >=0) 
					 && (abs(result[i][2].y -result[i][3].y) <=25))
					 || ((abs(result[i][3].x -result[i][0].x) >=0)  
					 && (abs(result[i][3].x -result[i][0].x) <=25)) 
					 && ((abs(result[i][3].y -result[i][0].y) >=0)  
					 && (abs(result[i][3].y -result[i][0].y) <=25)) 
					 || ((abs(result[i][0].x -result[i][2].x) >=0)  
					 && (abs(result[i][0].x -result[i][2].x) <=25)) 
					 && ((abs(result[i][0].y -result[i][2].y) >=0)  
					 && (abs(result[i][0].y -result[i][2].y) <=25)) 
					 || ((abs(result[i][1].x -result[i][3].x) >=0)  
					 && (abs(result[i][1].x -result[i][3].x) <=25)) 
					 && ((abs(result[i][1].y -result[i][3].y) >=0)  
					 && (abs(result[i][1].y -result[i][3].y) <=25)))
				{//8
				}//8
				else
				{//11
					
					int x_coord1, y_coord1;	
					/// Get the moments
  					Moments mu; 
					mu = moments( result[i], false );	
					x_coord1 = mu.m10/mu.m00;
					x_coord1 = x_coord1 + 13;
					y_coord1 = mu.m01/mu.m00;
					y_coord1 = y_coord1 + 3;
				line(image_shape2,Point(x_coord1 + 5,y_coord1),Point(x_coord1-5,y_coord1), Scalar(0,255,0), 1, 8, 0  );
        	  		line(image_shape2,Point(x_coord1,y_coord1 + 5),Point(x_coord1,y_coord1-5), Scalar(0,255,0), 1, 8, 0  );
					imshow("My Window", image_shape2);
					Image_Segment_Function(x_coord1, y_coord1);	
					if(flag || flag2 || flag4)
					{//9
						//flag = 0;
						flag4 = 0;
						flag2 = 0;
						Rect boundRect1;
						rectcount++;
						cout <<"Rectangle "<<" ";
						cout <<"contour point "<<" ";
						for(int j=0; j<4; ++j)
    						{
						 cout << result[i][j]<<" ";
						}

                                               // if()
							
						cout <<endl;
						flag = 0;
						int yval_count =0;
						int y_array[2];
						int offset_x =0, offset_y =0;	
						int minx_1 = std::min(result[i][0].x,result[i][1].x);
						int minx_2 = std::min(result[i][2].x,result[i][3].x);
						int minx = std::min(minx_1,minx_2);
						int max_1 = std::max(result[i][0].x,result[i][1].x);
						int max_2 = std::max(result[i][2].x,result[i][3].x);
						int max_x = std::max(max_1,max_2);
						int maxy_1 = std::max(result[i][0].y,result[i][1].y);
						int maxy_2 = std::max(result[i][2].y,result[i][3].y);
						int max_y = std::max(maxy_1,maxy_2);	
						int y1 =0, y2 =0, yflag = 0;
						for(int j=0; j<4; ++j)
    						{
							if((abs(result[i][j].x - minx) >= 0) && (abs(result[i][j].x - minx) <= 10)){
								y_array[yval_count] = result[i][j].y;
								yval_count++;
								
							}
						}
						int miny = std::min(y_array[0],y_array[1]);
						offset_x =  max_x - minx;
						offset_y = max_y - miny;
						cout<<"minx "<<minx<<"miny "<<miny<<endl;
						cout<<"offsetx "<<offset_x<<"offsety "<<offset_y<<endl;
						//Rect Rec(minx , miny,offset_x , offset_y);
						boundRect1 = boundingRect(Mat(result[i]));
						Roi1 = src(boundRect1);
		  			       // Roi1 = src(Rec);
						string name1;
						ostringstream oss2;
						oss2 << ROIcount;
						name1 = "ROI_rect";
						name1 +=oss2.str()+".jpg";
						//imwrite("RoI_rect",Roi1);
						imwrite(name1.c_str(),Roi1);
		  				namedWindow("ROI",WINDOW_NORMAL);  
                  				//imshow("ROI_rect", Roi1);
						line(image_shape2,result[i][0],result[i][1], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][1],result[i][2], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][2],result[i][3], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][3],result[i][0], Scalar(0,255,0), 2, 8, 0  ); 							imshow("My Window", image_shape2);
						ROIcount++;
                                                
					}//9
					else{//10
						cout <<"Color none"<<endl;
						}//10
				}//11
		    	}//12
			else if(result[i].size() == 8)
			{
				
				if((((abs(result[i][0].x -result[i][1].x) >=0) 
					&& (abs(result[i][0].x -result[i][1].x) <=25))
					&& ((abs(result[i][1].x -result[i][2].x) >=0) 
					&& (abs(result[i][1].x -result[i][2].x) <=25))
					&& ((abs(result[i][2].x -result[i][3].x) >=0) 
					&& (abs(result[i][2].x -result[i][3].x) <=25))
					&& ((abs(result[i][3].x -result[i][4].x) >=0) 
					&& (abs(result[i][3].x -result[i][4].x) <=25)) 
					&& ((abs(result[i][4].x -result[i][5].x) >=0) 
					&& (abs(result[i][4].x -result[i][5].x) <=25))
					&& ((abs(result[i][5].x -result[i][6].x) >=0) 
					&& (abs(result[i][5].x -result[i][6].x) <=25))
					&& ((abs(result[i][6].x -result[i][7].x) >=0) 
					&& (abs(result[i][6].x -result[i][7].x) <=25)))
					|| (((abs(result[i][0].y -result[i][1].y) >=0) 
					&& (abs(result[i][0].y -result[i][1].y) <=25)) 
					&& ((abs(result[i][1].y -result[i][2].y) >=0) 
					&& (abs(result[i][1].y -result[i][2].y) <=25))
					&& ((abs(result[i][2].y -result[i][3].y) >=0)
					&& (abs(result[i][2].y -result[i][3].y) <=25))
					&& ((abs(result[i][3].x -result[i][4].x) >=0) 
					&& (abs(result[i][3].x -result[i][4].x) <=25)) 
					&& ((abs(result[i][4].x -result[i][5].x) >=0) 
					&& (abs(result[i][4].x -result[i][5].x) <=25))
					&& ((abs(result[i][5].x -result[i][6].x) >=0) 
					&& (abs(result[i][5].x -result[i][6].x) <=25))
					&& ((abs(result[i][6].x -result[i][7].x) >=0) 
					&& (abs(result[i][6].x -result[i][7].x) <=25))))
					{
					}
				/* else if(((abs(result[i][0].x -result[i][1].x) >=0)
					&& (abs(result[i][0].x -result[i][1].x) <=25))
					&& ((abs(result[i][1].x -result[i][2].x) >=0) 
					&& (abs(result[i][1].x -result[i][2].x) <=25))
					&& ((abs(result[i][2].x -result[i][3].x) >=0) 
					&& (abs(result[i][2].x -result[i][3].x) <=25))
					&& ((abs(result[i][3].x -result[i][4].x) >=0) 
					&& (abs(result[i][3].x -result[i][4].x) <=25))
					|| ((abs(result[i][1].x -result[i][2].x) >=0)
					&& (abs(result[i][1].x -result[i][2].x) <=25))
					&& ((abs(result[i][2].x -result[i][3].x) >=0) 
					&& (abs(result[i][2].x -result[i][3].x) <=25))
					&& ((abs(result[i][3].x -result[i][4].x) >=0) 
					&& (abs(result[i][3].x -result[i][4].x) <=25))
					&& ((abs(result[i][4].x -result[i][5].x) >=0) 
					&& (abs(result[i][4].x -result[i][5].x) <=25))
					|| ((abs(result[i][2].x -result[i][3].x) >=0)
					&& (abs(result[i][2].x -result[i][3].x) <=25))
					&& ((abs(result[i][3].x -result[i][4].x) >=0) 
					&& (abs(result[i][3].x -result[i][4].x) <=25))
					&& ((abs(result[i][4].x -result[i][5].x) >=0) 
					&& (abs(result[i][4].x -result[i][5].x) <=25))
					&& ((abs(result[i][5].x -result[i][6].x) >=0) 
					&& (abs(result[i][5].x -result[i][6].x) <=25))
					|| ((abs(result[i][3].x -result[i][4].x) >=0)
					&& (abs(result[i][3].x -result[i][4].x) <=25))
					&& ((abs(result[i][4].x -result[i][5].x) >=0) 
					&& (abs(result[i][4].x -result[i][5].x) <=25))
					&& ((abs(result[i][5].x -result[i][6].x) >=0) 
					&& (abs(result[i][5].x -result[i][6].x) <=25))
					&& ((abs(result[i][6].x -result[i][7].x) >=0) 
					&& (abs(result[i][6].x -result[i][7].x) <=25))
					|| ((abs(result[i][0].x -result[i][1].x) >=0)
					&& (abs(result[i][0].x -result[i][1].x) <=25))
					&& ((abs(result[i][1].x -result[i][3].x) >=0) 
					&& (abs(result[i][1].x -result[i][3].x) <=25))
					&& ((abs(result[i][3].x -result[i][5].x) >=0) 
					&& (abs(result[i][3].x -result[i][5].x) <=25))
					&& ((abs(result[i][5].x -result[i][7].x) >=0) 
					&& (abs(result[i][5].x -result[i][7].x) <=25))
					|| ((abs(result[i][0].x -result[i][1].x) >=0)
					&& (abs(result[i][0].x -result[i][1].x) <=25))
					&& ((abs(result[i][1].x -result[i][2].x) >=0) 
					&& (abs(result[i][1].x -result[i][2].x) <=25))
					&& ((abs(result[i][2].x -result[i][4].x) >=0) 
					&& (abs(result[i][2].x -result[i][4].x) <=25))
					&& ((abs(result[i][4].x -result[i][5].x) >=0) 
					&& (abs(result[i][4].x -result[i][5].x) <=25))
					|| ((abs(result[i][0].x -result[i][1].x) >=0)
					&& (abs(result[i][0].x -result[i][1].x) <=25))
					&& ((abs(result[i][1].x -result[i][5].x) >=0) 
					&& (abs(result[i][1].x -result[i][5].x) <=25))
					&& ((abs(result[i][5].x -result[i][6].x) >=0) 
					&& (abs(result[i][5].x -result[i][6].x) <=25))
					&& ((abs(result[i][6].x -result[i][7].x) >=0) 
					&& (abs(result[i][6].x -result[i][7].x) <=25))
					|| ((abs(result[i][1].x -result[i][2].x) >=0)
					&& (abs(result[i][1].x -result[i][2].x) <=25))
					&& ((abs(result[i][2].x -result[i][3].x) >=0) 
					&& (abs(result[i][2].x -result[i][3].x) <=25))
					&& ((abs(result[i][3].x -result[i][5].x) >=0) 
					&& (abs(result[i][3].x -result[i][5].x) <=25))
					&& ((abs(result[i][5].x -result[i][6].x) >=0) 
					&& (abs(result[i][5].x -result[i][6].x) <=25)))
					{
					}
				else if(((abs(result[i][0].x -result[i][1].x) >=0)
					&& (abs(result[i][0].x -result[i][1].x) <=25))
					&& ((abs(result[i][1].x -result[i][2].x) >=0) 
					&& (abs(result[i][1].x -result[i][2].x) <=25))
					&& ((abs(result[i][2].x -result[i][3].x) >=0) 
					&& (abs(result[i][2].x -result[i][3].x) <=25))
					||((abs(result[i][3].x -result[i][4].x) >=0)
					&& (abs(result[i][3].x -result[i][4].x) <=25))
					&& ((abs(result[i][4].x -result[i][6].x) >=0) 
					&& (abs(result[i][4].x -result[i][6].x) <=25))
					&& ((abs(result[i][6].x -result[i][7].x) >=0) 
					&& (abs(result[i][6].y -result[i][7].x) <=25))
					||((abs(result[i][3].y -result[i][4].y) >=0)
					&& (abs(result[i][3].y -result[i][4].y) <=25))
					&& ((abs(result[i][4].y -result[i][6].y) >=0) 
					&& (abs(result[i][4].y -result[i][6].y) <=25))
					&& ((abs(result[i][6].y -result[i][7].y) >=0) 
					&& (abs(result[i][6].y -result[i][7].y) <=25))
				        ||((abs(result[i][0].y -result[i][1].y) >=0)
					&& (abs(result[i][0].y -result[i][1].y) <=25))
					&& ((abs(result[i][1].y -result[i][2].y) >=0) 
					&& (abs(result[i][1].y -result[i][2].y) <=25))
					&& ((abs(result[i][2].y -result[i][3].y) >=0) 
					&& (abs(result[i][2].y -result[i][3].y) <=25)))
					{
						
					}*/
				
				else
				{
					Rect boundRect;
					Moments mu1; 
					mu1 = moments( result[i], false );	
					int x_coord1 = mu1.m10/mu1.m00;
					int y_coord1 = mu1.m01/mu1.m00;
				line(image_shape2,Point(x_coord1 + 5,y_coord1),Point(x_coord1-5,y_coord1), Scalar(0,255,0), 1, 8, 0  );
        	  		line(image_shape2,Point(x_coord1,y_coord1 + 5),Point(x_coord1,y_coord1-5), Scalar(0,255,0), 1, 8, 0  );
					imshow("My Window", image_shape2);
					Image_Segment_Function(x_coord1, y_coord1);
					if(flag1)
					{	
						flag1 = 0;
						//flag2 = 0;
						cout <<"octagon "<<" ";
						for(int j=0; j<8; ++j)
    						{
							cout << result[i][j]<<" ";
						}	
						cout <<endl;
						line(image_shape2,result[i][0],result[i][1], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][1],result[i][2], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][2],result[i][3], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][3],result[i][4], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][4],result[i][5], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][5],result[i][6], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][6],result[i][7], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][7],result[i][0], Scalar(0,255,0), 2, 8, 0  );	
						cout <<endl;
						boundRect = boundingRect(Mat(result[i]));
		  				Roi2 = src(boundRect);
						string name1;
						ostringstream oss3;
						oss3 << ROIcount;
						name1 = "ROI_oct";
						name1 += oss3.str()+".jpg";
						imwrite(name1.c_str(),Roi2);
						//imwrite("ROI_oct",Roi2);
		  				namedWindow("ROI_oct",WINDOW_NORMAL);  
                  				imshow("ROI_oct", Roi2);
						imshow("My Window", image_shape2);
						ROIcount++;
						hexcount++;
					}
					else{
						cout <<"Color none"<<endl;
						}
					
            				}
				
				}
				/*else if(result[i].size() == 5){//1

				if (((abs(result[i][0].y -result[i][1].y) >=0) 
					&& (abs(result[i][0].y -result[i][1].y) <=25)) 
					&& ((abs(result[i][1].y -result[i][2].y) >=0) 
					&& (abs(result[i][1].y -result[i][2].y) <=25))
					&& ((abs(result[i][2].y -result[i][3].y) >=0)
					&& (abs(result[i][2].y -result[i][3].y) <=25))
					&& ((abs(result[i][3].y -result[i][4].y) >=0)
					&& (abs(result[i][3].y -result[i][4].y) <=25))
					||((abs(result[i][1].y -result[i][2].y) >=0) 
					&& (abs(result[i][1].y -result[i][2].y) <=25)) 
					&& ((abs(result[i][2].y -result[i][3].y) >=0) 
					&& (abs(result[i][2].y -result[i][3].y) <=25))
					&& ((abs(result[i][3].y -result[i][4].y) >=0)
					&& (abs(result[i][3].y -result[i][4].y) <=25))
					&& ((abs(result[i][4].y -result[i][0].y) >=0)
					&& (abs(result[i][4].y -result[i][0].y) <=25))
					||((abs(result[i][0].x -result[i][1].x) >=0) 
					&& (abs(result[i][0].x -result[i][1].x) <=25)) 
					&& ((abs(result[i][1].x -result[i][2].x) >=0) 
					&& (abs(result[i][1].x -result[i][2].x) <=25))
					&& ((abs(result[i][2].x -result[i][3].x) >=0)
					&& (abs(result[i][2].x -result[i][3].x) <=25))
					&& ((abs(result[i][3].x -result[i][4].x) >=0)
					&& (abs(result[i][3].x -result[i][4].x) <=25))
					||((abs(result[i][1].x -result[i][2].x) >=0) 
					&& (abs(result[i][1].x -result[i][2].x) <=25)) 
					&& ((abs(result[i][2].x -result[i][3].x) >=0) 
					&& (abs(result[i][2].x -result[i][3].x) <=25))
					&& ((abs(result[i][3].x -result[i][4].x) >=0)
					&& (abs(result[i][3].x -result[i][4].x) <=25))
					&& ((abs(result[i][4].x -result[i][0].x) >=0)
					&& (abs(result[i][4].x -result[i][0].x) <=25)))
                                	{ //2 
	
					}//2
				else{//3
					Rect boundRect;
					Moments mu1; 
					mu1 = moments( result[i], false );	
					int x_coord1 = mu1.m10/mu1.m00;
					int y_coord1 = mu1.m01/mu1.m00;
				line(image_shape2,Point(x_coord1 + 5,y_coord1),Point(x_coord1-5,y_coord1), Scalar(0,255,0), 1, 8, 0  );
        	  		line(image_shape2,Point(x_coord1,y_coord1 + 5),Point(x_coord1,y_coord1-5), Scalar(0,255,0), 1, 8, 0  );
					imshow("My Window", image_shape2);
					Image_Segment_Function(x_coord1, y_coord1);
					if(flag)
					{ //4	
						flag = 0;
						//flag2 = 0;
						cout <<"pentagon "<<" ";
						for(int j=0; j<8; ++j)
    						{
							cout << result[i][j]<<" ";
						}	
						cout <<endl;
						line(image_shape2,result[i][0],result[i][1], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][1],result[i][2], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][2],result[i][3], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][3],result[i][4], Scalar(0,255,0), 2, 8, 0  );
						line(image_shape2,result[i][4],result[i][5], Scalar(0,255,0), 2, 8, 0  );	
						cout <<endl;
						boundRect = boundingRect(Mat(result[i]));
		  				Roi3 = src(boundRect);
						string name1;
						ostringstream oss4;
						oss4 << ROIcount;
						name1 = "ROI_pent";
						name1 += oss4.str()+".jpg";
						imwrite(name1.c_str(),Roi3);
						//imwrite("ROI_oct",Roi2);
		  				namedWindow("ROI_pent",WINDOW_NORMAL);  
                  				imshow("ROI_pent", Roi3);
						imshow("My Window", image_shape2);
						ROIcount++;
						hexcount++;
					
   					}//4	
					else{//5
						cout <<"Color none"<<endl;
						}//5
			}//3
		}//1*/
			
		
		}//13

		cout <<"total no of proper rect "<< rectcount<<endl;
		cout <<"total no of proper oct "<< hexcount<<endl;
		//imwrite("RoI_rect",Roi1);
		//namedWindow("ROI_rect",WINDOW_NORMAL);  
                //imshow("ROI_rect", Roi1);
		//imwrite("ROI_oct",Roi2);
		//namedWindow("ROI_oct",WINDOW_NORMAL);  
                //imshow("ROI_oct", Roi2);
		//imshow("My Window", image_shape2);
		//setMouseCallback("My Window", MouseCallBackFunc, 0 );		
	}
}

/*Main Function*/

int main(int argc, char **argv)
{	
	source=VideoCapture(argv[1]);
	createTrackbars();
	if(!source.isOpened()) { // check if we succeeded
		cout<<"Open Failure"<<endl;        
		return -1;
	}
	int fc=source.get(CAP_PROP_FRAME_COUNT);
	namedWindow("Video",WINDOW_NORMAL);	
	createTrackbar( "seek", "Video", &pos, fc);
	setMouseCallback("Video", onMouse, 0 );
	cout<<"frame height" << source.get(CV_CAP_PROP_FRAME_HEIGHT)<<"frame width "<<source.get(CV_CAP_PROP_FRAME_WIDTH)<<endl	;
	for(;;){

		source>>frame;
		pos=source.get(CAP_PROP_POS_FRAMES);
		if(frame.empty()){
			cout<<"EOF"<<endl;
			break;
		}
		setTrackbarPos("seek", "Video", pos);	
		imshow("Video", frame);
		waitKey(70);
	}
	source.release();
return 0;
}
