#pragma once

#include "ofMain.h"

struct pointPair{
    ofPoint point1;
    ofPoint point2;
};

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void exportFile();
    void exportFileForStruct();
    void importFile();
    void importFileForStruct();
    
    
    double getColorDistance(ofColor color1, ofColor color2);
	
    ofImage image1, image2;
		ofImage originalImage1, originalImage2;
    bool drawImageSwitch;
    int counter;
    vector<pointPair> pointPairVector;
    
    bool enableDuplicate;
    vector<bool> usedOrNotVector;
    
    bool enableExchange;
    
    bool enableRandomExchange;
    vector<int> exchangeOrderVector;
		
		bool enableMultipixelDrawing;
		bool saveToFile;
};
