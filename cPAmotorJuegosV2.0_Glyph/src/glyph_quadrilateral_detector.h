/*
 * glyph_quadrilateral_detector.h
 *
 *  Created on: 6 Mar 2022
 *      Author: Francisco Dominguez
 */

#pragma once
#include "glyph_contour.h"

class QuadrilateralDetector{
public:
	RNG rng;
	Mat img,gray,edges,drawing;
	vector<vector<Point> > contours;
	Contour approx;
	Mat square;
	vector<Contour> quads;
public:
	Mat &getImage(){return img;}
	//Histogram equalization
	/*
	void applyCLAHE(Mat &srcArry, Mat &dstArry)
	{
	    // READ RGB color image and convert it to Lab
	    cv::Mat bgr_image = srcArry;
	    cv::Mat lab_image;
	    cv::cvtColor(bgr_image, lab_image, CV_BGR2Lab);

	    // Extract the L channel
	    std::vector<cv::Mat> lab_planes(3);
	    cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

	    // apply the CLAHE algorithm to the L channel
	    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	    clahe->setClipLimit(4);
	    cv::Mat dst;
	    clahe->apply(lab_planes[0], dst);
	    //normalize(lab_planes[0],dst,0,255,NORM_MINMAX,CV_8UC1);

	    // Merge the the color planes back into an Lab image
	    dst.copyTo(lab_planes[0]);
	    cv::merge(lab_planes, lab_image);

	   // convert back to RGB
	   cv::Mat image_clahe;
	   cv::cvtColor(lab_image, image_clahe, CV_Lab2BGR);

	   // display the results  (you might also want to see lab_planes[0] before and after).
	   cv::imshow("image original", bgr_image);
	   cv::imshow("image CLAHE", image_clahe);
	   dstArry=image_clahe;
	   //cv::waitKey();
	}*/
	/*
	void lds_test(){
	    //String filename = "/home/francisco/Pictures/Webcam/2020-12-10-192656.jpg";
	    //String filename = "/home/francisco/Pictures/Webcam/2020-12-11-091815.jpg";
	    String filename = "/home/francisco/Pictures/Webcam/2020-12-11-093613.jpg";
	    bool useRefine = true;
	    bool useCanny = false;
	    bool overlay = true;

	    Mat image = imread(filename, IMREAD_GRAYSCALE);

	    if( image.empty() ){
	        cout << "Unable to load " << filename<<endl;
	        return;
	    }

	    imshow("Source Image", image);

	    if (useCanny){
	        Canny(image, image, 50, 200, 3); // Apply Canny edge detector
	    }

	    // Create and LSD detector with standard or no refinement.
	    Ptr<LineSegmentDetector> ls = useRefine ? createLineSegmentDetector(LSD_REFINE_STD) : createLineSegmentDetector(LSD_REFINE_NONE);

	    double start = double(getTickCount());
	    vector<Vec4f> lines_std;

	    // Detect the lines
	    ls->detect(image, lines_std);

	    double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	    std::cout << "It took " << duration_ms << " ms." << std::endl;

	    // Show found lines
	    if (!overlay || useCanny)
	    {
	        image = Scalar(0, 0, 0);
	    }

	    ls->drawSegments(image, lines_std);

	    String window_name = useRefine ? "Result - standard refinement" : "Result - no refinement";
	    window_name += useCanny ? " - Canny edge detector used" : "";

	    imshow(window_name, image);

	    waitKey(0);
	}*/
	Mat &detectEdges(Mat &img){
		// Stage 2: Detect edges in image
		Mat gblur;
		//applyCLAHE(img,img);
		cvtColor(img,gray, COLOR_BGR2GRAY);
		GaussianBlur(gray,gblur, Size(5,5), 0);
		Canny(gray,edges, 50, 150);
        // dilate canny output to remove potential
        // holes between edge segments
		//lines = HoughLinesP(edges,1,np.pi/180,100,minLineLength,maxLineGap)
        dilate(edges, edges, Mat(), Point(-1,-1));
        erode(edges,edges,Mat(),Point(-1,-1));
        //dilate(edges, edges, Mat(), Point(-1,-1));
           // Mat edgesc;
            //cvtColor(edges,edgesc,COLOR_GRAY2BGR);
            /*
             std::vector<cv::Vec4i> slines;
             cv::HoughLinesP(edges, slines, 0.125, CV_PI/180, 25, 32, 2);
             for(Vec4i v:slines){
            	double dx=v[2]-v[0];
            	double dy=v[3]-v[1];
            	double l=sqrt(dx*dx+dy*dy);
            	if(l<50)
            		cv::line(edgesc,Point(v[0],v[1]),Point(v[2],v[3]),Scalar(255,255,0),1);
            }
            vector<Vec2f> lines;
            HoughLines(edges,lines ,0.5,CV_PI/180,50,0,0);
             for( size_t i = 0; i < lines.size(); i++ )
            {
               float rho = lines[i][0], theta = lines[i][1];
               Point pt1, pt2;
               double a = cos(theta), b = sin(theta);
               double x0 = a*rho, y0 = b*rho;
               pt1.x = cvRound(x0 + 1000*(-b));
               pt1.y = cvRound(y0 + 1000*(a));
               pt2.x = cvRound(x0 - 1000*(-b));
               pt2.y = cvRound(y0 - 1000*(a));
               line( edgesc, pt1, pt2, Scalar(255,0,255), 1, CV_AA);
             }*/
    		//imshow("canny",edgesc);
      return edges;
	}
	bool isQuadrilateralContour(vector<Point> &contour){
		double perimeter=arcLength(contour,true);
		cv::approxPolyDP(contour,approx,0.01*perimeter,true);
		return approx.size()==4 &&
	           fabs(contourArea(approx)) > 16*16 &&
	           isContourConvex(approx) ;
	}
	// comparison function object
	static bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
	    double i = fabs( contourArea(cv::Mat(contour1)) );
	    double j = fabs( contourArea(cv::Mat(contour2)) );
	    return ( i < j );
	}
	void sortQuadrilateralPoints(){
		sort(approx.begin(),approx.end(),[](Point2f &p0,Point2f &p1){return p0.y<p1.y;});
		Point2f &p0=approx[0];
		Point2f &p1=approx[1];
		if(p0.x>p1.x)
			swap(p0,p1);
		Point2f &p2=approx[2];
		Point2f &p3=approx[3];
		if(p2.x<p3.x)
			swap(p2,p3);
	}
	void drawQuadrilateral(Mat &drawing,Contour c){
	  	   cv::line(drawing,c[0],c[1],Scalar(255,0,0),3);
	  	   cv::line(drawing,c[1],c[2],Scalar(255,255,0),3);
	  	   cv::line(drawing,c[2],c[3],Scalar(255,0,255),3);
	  	   cv::line(drawing,c[3],c[0],Scalar(  0,0,255),3);
	}
	void drawQuadrilaterals(Mat &drawing){
		for(Contour c:quads)
			drawQuadrilateral(drawing,c);
	}
	vector<Contour> &detect(Mat &img){
		img.copyTo(this->img);
		// Stage 3: Find contours
		vector<Vec4i> hierarchy;

		//clean quads
		quads.clear();

		//Detect edges
		detectEdges(img);

		// Find contours
		//cv::findContours( edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
		cv::findContours( edges, contours, hierarchy, RETR_CCOMP , CHAIN_APPROX_SIMPLE, Point(0, 0) );
		sort(contours.begin(),contours.end(),compareContourAreas);
		reverse(contours.begin(),contours.end());
        /// Draw contours
		cvtColor(edges,drawing,COLOR_GRAY2BGR);
		addWeighted(img,0.1,drawing,0.9,0.0,drawing);
		for(unsigned int i = 0; i< (contours.size()>10?10:contours.size()); i++ ){
		     Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		     if(isQuadrilateralContour(contours[i])){
		    	 drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
	             //imshow("drawing",drawing);
		         sortQuadrilateralPoints();
		         quads.push_back(approx);
		     }
		 }
		 return quads;
	}
};



