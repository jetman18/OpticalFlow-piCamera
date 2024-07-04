#include <vector>
#include <ctime>
#include "serial.hpp"
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <sys/time.h>
#include <iostream>
#include "flow.hpp"
#include <raspicam/raspicam.h>
struct timeval startt,endd;
using namespace std;
#define IMAGE_WIDTH 100 // FRAMESIZE_QQVGA2 => 128x160
#define SEARCH_SIZE 12
#define FLOW_FEATURE_THRESHOLD 1
#define FLOW_VALUE_THRESHOLD 5000
int main ( int argc,char **argv ) {
    raspicam::RaspiCam Camera; //Cmaera object
    Camera.setWidth(100);
    Camera.setHeight(100);
    Camera.setFrameRate(30);
    Camera.setFormat(raspicam::RASPICAM_FORMAT_GRAY);
    //Open camera 
    cout<<"Opening Camera..."<<endl;
    if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}
    //wait a while until camera stabilizes
    cout<<"Sleeping for 3 secs"<<endl;
   // sleep(3);
    //capture
   // Camera.grab();
    uint32_t len = Camera.getImageBufferSize();
    cout<<len<<endl;
    sleep(3);    
    PX4Flow *px4 = new PX4Flow(IMAGE_WIDTH,SEARCH_SIZE,FLOW_FEATURE_THRESHOLD,FLOW_VALUE_THRESHOLD);
    //allocate memory
    unsigned char *data = new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_GRAY)];
    unsigned char *p_data = new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_GRAY)]; 
//gettimeofday(&startt,NULL);   
   serial sender(115200,"/dev/ttyS0"); 
   while(1){
       Camera.grab();
       Camera.retrieve (data);//get camera image
       float flow_rate_x, flow_rate_y;
       unsigned char quality = px4->compute_flow(p_data,data,0,0,0, &flow_rate_x, &flow_rate_y);
       memcpy(p_data,data,len);
       flow_rate_x *=10;
       flow_rate_y *=10;
       if(flow_rate_x>127)flow_rate_x=127;
       else if(flow_rate_x<-127)flow_rate_x=-127;
       if(flow_rate_y>127)flow_rate_y=127;
       else if(flow_rate_y<-127)flow_rate_y=-127;
       int xx=flow_rate_x;
       int yy=flow_rate_y;
       sender.send_data((unsigned char)xx,(unsigned char)yy,quality);
      // cout<<(int)xx<<endl;
    }
    //gettimeofday(&endd,NULL);
    //double tim = (((endd.tv_sec  - startt.tv_sec) * 1000 + (endd.tv_usec - startt.tv_usec)/1000.0) + 0.5)/1000.;   //save
    //cout<<tim<<endl;
    //free resrources    
    //delete data;
    return 0;
    }
