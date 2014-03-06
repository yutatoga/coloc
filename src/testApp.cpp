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
    counter = -1;
}

//--------------------------------------------------------------
void testApp::update(){
    counter++;
}

//--------------------------------------------------------------
void testApp::draw(){
//	if (drawImageSwitch) {
//		image1.draw(ofPoint(100, 100), image1.getWidth(), image1.getHeight());
//		image2.draw(ofPoint(520, 100), image2.getWidth(), image2.getHeight());
//		drawImageSwitch = false;
//        counter = 0;
//	}
//	for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
//        
//	}
    
    
//    unsigned char * pixels = image1.getPixels();
//    //画像を8ピクセルごとにスキャン
//    for (int i = 0; i < image1.height; i++){
//        for (int j = 0; j < image1.width; j++){
//            //RGBそれぞれのピクセルの明度を取得
//            unsigned char r = pixels[(i*(int)image1.getWidth()+j)*3];
//            unsigned char g = pixels[(i*(int)image1.getWidth()+j)*3+1];
//            unsigned char b = pixels[(i*(int)image1.getWidth()+j)*3+2];
//            //取得したRGB値をもとに、円を描画
//            //取得したピクセルの明るさを、円の半径に対応させている
//            ofPushStyle();
//            ofSetColor(r, g, b);
//            ofCircle(360 + j,20+i,1);
//            ofPopStyle();
//        }
//    }

    // 左の画像を右の画像に入れ替える
    int x = counter%(int)image1.getWidth();
    int y = counter/(int)image1.getWidth();
    
    ofColor targetColor = image1.getColor(x, y);
    image2.setColor(x, y, targetColor);
    image2.update();
    
    image1.draw(ofPoint(100, 100), image1.getWidth(), image1.getHeight());
    image2.draw(ofPoint(520, 100), image2.getWidth(), image2.getHeight());

    
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
