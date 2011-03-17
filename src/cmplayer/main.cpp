#include "application.hpp"
#include <QtCore/QDebug>
#include <QtCore/QMetaType>
//#include "libvlc.h"




int main(int argc, char **argv) {

//	IplImage *imgA = cvLoadImage( "blurtest.png", 1);
//	IplImage *imgB = cvCreateImage( cvGetSize( imgA), IPL_DEPTH_8U, 3);
//	const double angle  = 40.0; //회전각도
//	const double scale  = 1.0;  //크기
//	CvPoint2D32f center = cvPoint2D32f( imgA->width/2.0, imgA->height/2.0);//회전중심설정
//	CvMat *rot_mat = cvCreateMat( 2, 3, CV_32FC1);
//	cv2DRotationMatrix( center, angle, scale, rot_mat); // 메트릭스 변환
//	cvWarpAffine( imgA, imgB, rot_mat, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS); // 선형보간
//	cvSaveImage( "image_result8.bmp", imgB);
//	cvReleaseImage( &imgA );
//	cvReleaseImage( &imgB );
//	cvReleaseMat( &rot_mat);
//	return 0;




	Application app(argc, argv);
//	LibVlc::init();
	const int ret = app.exec();
//	LibVlc::release();
	return ret;
}
