#include <windows.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <iostream>



using namespace std;
using namespace cv;
void *__gxx_personality_v0;

int Gesture();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR lpCmdLine, int nCmdShow )
{
  MSG  msg ;
  WNDCLASS wc = {0};
  wc.lpszClassName = TEXT( "Hack e-Able" );
  wc.hInstance     = hInstance ;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.lpfnWndProc   = WndProc ;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);


  RegisterClass(&wc);
  CreateWindow( wc.lpszClassName, TEXT("Hack e-Able"),
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                150, 150, 400, 250, 0, 0, hInstance, 0);

  while( GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return (int) msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{


  switch(msg)
  {
	case WM_CREATE:
	{
        CreateWindow(TEXT("edit"), TEXT("Welcome to Hack e-Able !!"),
		             WS_VISIBLE | WS_CHILD ,
		             115, 20, 180, 25,
		             hwnd, (HMENU) 0, NULL, NULL);
        CreateWindow(TEXT("edit"), TEXT("An innovative solution to help differently able."),
		             WS_VISIBLE | WS_CHILD ,
		             40, 60, 300, 20,
		             hwnd, (HMENU) 0, NULL, NULL);

        CreateWindow(TEXT("edit"), TEXT("To help them to express their thoughts "),
		             WS_VISIBLE | WS_CHILD ,
		             40, 80, 300, 20,
		             hwnd, (HMENU) 0, NULL, NULL);

		CreateWindow(TEXT("edit"), TEXT("and communicate effectively."),
		             WS_VISIBLE | WS_CHILD ,
		             40, 100, 300, 20,
		             hwnd, (HMENU) 0, NULL, NULL);
		CreateWindow(TEXT("edit"), TEXT("Hence to make them independent."),
		             WS_VISIBLE | WS_CHILD ,
		             40, 120, 300, 20,
		             hwnd, (HMENU) 0, NULL, NULL);

        CreateWindow(TEXT("Button"), TEXT("Start"),
		             WS_VISIBLE | WS_CHILD ,
		             80, 150, 100, 25,
		             hwnd, (HMENU) 1, NULL, NULL);

	    CreateWindow(TEXT("Button"), TEXT("Quit"),
		             WS_VISIBLE | WS_CHILD ,
		             200, 150, 100, 25,
		             hwnd, (HMENU) 2, NULL, NULL);
	    break;
	}

      case WM_COMMAND:
      {
	   if (LOWORD(wParam) == 1) {
	       Gesture();
	   }

	   if (LOWORD(wParam) == 2) {
            cvDestroyAllWindows();
            PostQuitMessage(0);
              //exit(0);
              break;
	   }

	   break;
       }

      case WM_DESTROY:
      {
         cvDestroyAllWindows();
            PostQuitMessage(0);
            exit(0);
         break;
      }
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}




int Gesture()
{
	int c = 0;
	CvCapture* capture = cvCaptureFromCAM(0);
	if(!cvQueryFrame(capture))
	{
		cout<<"Video camera capture status: OK"<<endl;
	}
	else
	{
		cout<<"Video capture failed, please check the camera."<<endl;
	}

        CvSize sz = cvGetSize(cvQueryFrame( capture));
	cout << "Height & width of captured frame: " << sz.height <<" x " << sz.width;
	IplImage* src    = cvCreateImage( sz,8, 3 );
	IplImage* gray   = cvCreateImage( cvSize(270,270),8, 1 );

	while( c != 27)
	{
		src = cvQueryFrame(capture);
		cvSetImageROI(src, cvRect(340,100,270,270));
		cvCvtColor(src,gray,CV_BGR2GRAY);
		cvSmooth(gray,gray,CV_BLUR,(12,12),0);
		//cvNamedWindow( "Blur",1);cvShowImage( "Blur",gray);   // blur-not-clear
		cvThreshold(gray,gray,0,255,(CV_THRESH_BINARY_INV+CV_THRESH_OTSU));
		cvNamedWindow( "Threshold",1);cvShowImage( "Threshold",gray);  // black-white
		CvMemStorage* storage = cvCreateMemStorage();
		CvSeq* first_contour = NULL;
		CvSeq* maxitem=NULL;
		int cn=cvFindContours(gray,storage,&first_contour,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
		double area,max_area=0.0;
		CvSeq* ptr=0;
		//int maxn=0,n=0;
		if(cn>0)
		{
			for(ptr=first_contour;ptr!=NULL;ptr=ptr->h_next)
			{
				area=fabs(cvContourArea(ptr,CV_WHOLE_SEQ,0));
				if(area>max_area)
				{
					max_area=area;
					maxitem=ptr;
					//maxn=n;
				}
				// n++;
			}
			if(max_area > 1000)
			{
				CvPoint pt0;
				CvMemStorage* storage1 = cvCreateMemStorage();
				CvMemStorage* storage2 = cvCreateMemStorage(0);
				CvSeq* ptseq = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),sizeof(CvPoint), storage1 );
				CvSeq* hull;
				CvSeq* defects;
				for(int i = 0; i < maxitem->total; i++ )
				{
					CvPoint* p = CV_GET_SEQ_ELEM( CvPoint, maxitem, i );
					pt0.x = p->x;
					pt0.y = p->y;
					cvSeqPush( ptseq, &pt0 );
				}
				hull = cvConvexHull2( ptseq, 0, CV_CLOCKWISE, 0 );
				int hullcount = hull->total;
				defects= cvConvexityDefects(ptseq,hull,storage2  );
				// pt0 = **CV_GET_SEQ_ELEM( CvPoint*, hull, hullcount - 1 );
				// printf("** : %d :**",hullcount);
				CvConvexityDefect* defectArray;
				// int j=0;
				for(int i = 1; i <= hullcount; i++ )
				{
					CvPoint pt = **CV_GET_SEQ_ELEM( CvPoint*, hull, i );
					cvLine( src, pt0, pt, CV_RGB( 255, 0, 0 ), 1, CV_AA, 0 );
					pt0 = pt;
				}
				for( ; defects; defects = defects->h_next)
				{
					int nomdef = defects->total; // defect amount
					// outlet_float( m_nomdef, nomdef );
					// printf(" defect no %d \n",nomdef);
					if(nomdef == 0)
					continue;
					// Alloc memory for defect set.
					// fprintf(stderr,"malloc\n");
					defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*nomdef);
					// Get defect set.
					// fprintf(stderr,"cvCvtSeqToArray\n");
					cvCvtSeqToArray(defects,defectArray, CV_WHOLE_SEQ);
					// Draw marks for all defects.
					int con=0;
					for(int i=0; i<nomdef; i++)
					{
						if(defectArray[i].depth > 40 )
						{
							con=con+1;
							// printf(" defect depth for defect %d %f \n",i,defectArray[i].depth);
							cvLine(src, *(defectArray[i].start), *(defectArray[i].depth_point),CV_RGB(255,255,0),1, CV_AA, 0 );
							cvCircle( src, *(defectArray[i].depth_point), 5, CV_RGB(0,0,255), 2, 8,0);
							cvCircle( src, *(defectArray[i].start), 5, CV_RGB(0,255,0), 2, 8,0);
							cvLine(src, *(defectArray[i].depth_point), *(defectArray[i].end),CV_RGB(0,255,255),1, CV_AA, 0 );
							cvDrawContours(src,defects,CV_RGB(0,0,0),CV_RGB(255,0,0),-1,CV_FILLED,8);
						}
					}
					// cout<<con<<"\n";
					char txt[40]="";
					if(con==1)
					{
						char txt1[]="It's 1";
						strcat(txt,txt1);
					}
					else if(con==2)
					{
						char txt1[]="It's 2";
						strcat(txt,txt1);
					}
					else if(con==3)
					{
						char txt1[]="It's 3";
						strcat(txt,txt1);
					}
					else if(con==4)
					{
						char txt1[]="It's 5";
						strcat(txt,txt1);
					}
					else
					{
						char txt1[]="Sorry, unable to detect.";
						strcat(txt,txt1);
					}
			//		cvNamedWindow( "contour",1);cvShowImage( "contour",src);
					cvResetImageROI(src);
					CvFont font;
					cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.5, 1.5, 0, 5, CV_AA);
					cvPutText(src, txt, cvPoint(50, 50), &font, cvScalar(0, 0, 255, 0));
					// j++;
					// Free memory.
					free(defectArray);
				}
				cvReleaseMemStorage( &storage1 );
				cvReleaseMemStorage( &storage2 );
			}
		}
		cvReleaseMemStorage( &storage );
		cvNamedWindow( "Welcome to Hack e-Able!!",1);cvShowImage( "Welcome to Hack e-Able!!",src);
		c = cvWaitKey(100);
	}
	cvReleaseCapture( &capture);
	cvDestroyAllWindows();
}



