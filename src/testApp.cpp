#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetBackgroundAuto(false);
	image1.loadImage("toga.jpg");
	image2.loadImage("monalisa.jpg");
	//image1とimage2は、同じサイズであることを前提とする
	for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
		changeRecordVector.push_back(false);
	}
	drawImageSwitch = false;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	if (drawImageSwitch) {
		image1.draw(ofPoint(100, 100), image1.getWidth(), image1.getHeight());
		image2.draw(ofPoint(520, 100), image2.getWidth(), image2.getHeight());
		drawImageSwitch = false;
	}
//	for (int i=0; i<image1.getWidth()*image1.getHeight(); <#increment#>) {
//		<#statements#>
//	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	drawImageSwitch = true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
