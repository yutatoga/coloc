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
    void exportFileForStruct(vector<pointPair> ppv);
    void importFile();
    void importFileForStruct(vector<pointPair> *ppv);
    void setupCurrentPointVector(vector<pointPair> ppv);
		
		void moveUpDownLeftRight();
		
    
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
    
    vector<pointPair> readPointPairVector;
		
		// アニメーション用
		bool dataReaded;
		int startAnimationFrameNum;
		bool animationToOriginal;
		bool animationToExchange;
		vector<ofPoint> currentPointVector;
		vector<ofPoint> currentRectRightPointVector;
		vector<ofPoint> currentRectLeftPointVector;
		ofPoint currentLeftImageOrigin;
		float marginBetweenImages;
};
