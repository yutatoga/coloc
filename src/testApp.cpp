#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(ofColor::black);
    //    ofSetBackgroundAuto(false);
    
    image1.loadImage("toga.jpg");
    image2.loadImage("monalisa.jpg");
    //image1とimage2は、同じサイズであることを前提とする
    drawImageSwitch = false;
    counter = -1;
    for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
        usedOrNotVector.push_back(false);
    }
    enableDuplicate = false;
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
    ofPoint referencePoint(counter%(int)image1.getWidth(), counter/(int)image1.getWidth());
    ofColor referenceColor = image1.getColor(referencePoint.x, referencePoint.y);
    double minimumDistance = image1.getWidth()+image2.getHeight();
    ofPoint minimumDistancePoint;
    unsigned char * checkImagePixels = image2.getPixels();
    // 最小の距離の色を調べる
    for (int i=0; i<image1.getHeight(); i++) {
        for (int j=0; j<image1.getWidth(); j++) {
            if (!enableDuplicate) {
                // 重複を許さない
                if (!usedOrNotVector[i*(int)image2.getWidth()+j]) {
                    ofColor checkColor = ofColor(checkImagePixels[(i*(int)image2.getWidth()+j)*3],
                                                 checkImagePixels[(i*(int)image2.getWidth()+j)*3+1],
                                                 checkImagePixels[(i*(int)image2.getWidth()+j)*3+2]);
                    double checkDistance = getColorDistance(referenceColor, checkColor);
                    if (checkDistance < minimumDistance) {
                        if (!usedOrNotVector[i*(int)image2.getWidth()+j]) {
                            minimumDistance = checkDistance;
                            minimumDistancePoint = ofPoint(j, i);
                        }
                        // 同じ色の重複を許す
                        minimumDistance = checkDistance;
                        minimumDistancePoint = ofPoint(j, i);
                    }
                }
            } else{
                ofColor checkColor = ofColor(checkImagePixels[(i*(int)image2.getWidth()+j)*3],
                                             checkImagePixels[(i*(int)image2.getWidth()+j)*3+1],
                                             checkImagePixels[(i*(int)image2.getWidth()+j)*3+2]);
                double checkDistance = getColorDistance(referenceColor, checkColor);
                if (checkDistance < minimumDistance) {
                    if (!usedOrNotVector[i*(int)image2.getWidth()+j]) {
                        minimumDistance = checkDistance;
                        minimumDistancePoint = ofPoint(j, i);
                    }
                    // 同じ色の重複を許す
                    minimumDistance = checkDistance;
                    minimumDistancePoint = ofPoint(j, i);
                }
            }
        }
    }
    //最も近い距離の色の組み合わせをベクターに保存
    pointPair tempPointPair;
    tempPointPair.point1 = referencePoint;
    tempPointPair.point2 = minimumDistancePoint;
    
    // 重複を許さない
    if (!enableDuplicate) {
        //再利用しないために、使用したPointを保存
        usedOrNotVector[minimumDistancePoint.y*(int)image2.getWidth()+minimumDistancePoint.x] = true;
    }
    pointPairVector.push_back(tempPointPair);
//    ofLog(OF_LOG_NOTICE, "X:"+ofToString(pointPairVector.back().point2.x)+" Y:"+ofToString(pointPairVector.back().point2.y));
    
    image1.setColor(pointPairVector.back().point1.x, pointPairVector.back().point1.y, image2.getColor(pointPairVector.back().point2.x, pointPairVector.back().point2.y));
    image1.update();
    
    //		image2.setColor(referencePoint.x, referencePoint.y , targetColor);
    //    image2.update();
    
    image1.draw(ofPoint(ofGetWidth()/4.-image1.getWidth()/2.+100, (ofGetHeight()-image1.getHeight())/2.), image1.getWidth(), image1.getHeight());
    image2.draw(ofPoint(ofGetWidth()/4.*3-image2.getWidth()/2.-100 , (ofGetHeight()-image2.getHeight())/2.), image2.getWidth(), image2.getHeight());
    
    //hightLihgting
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofSetLineWidth(1);
    ofLine(pointPairVector.back().point1.x+(ofGetWidth()/4.-image1.getWidth()/2.+100), pointPairVector.back().point1.y+((ofGetHeight()-image1.getHeight())/2.),
           pointPairVector.back().point2.x+(ofGetWidth()/4.*3-image2.getWidth()/2.-100), pointPairVector.back().point2.y+((ofGetHeight()-image2.getHeight())/2.));
    ofPopStyle();
}

double testApp::getColorDistance(ofColor color1, ofColor color2){
    return sqrt(double((color1.r-color2.r)*(color1.r-color2.r)+(color1.g-color2.g)*(color1.g-color2.g)+(color1.b-color2.b)*(color1.b-color2.b)));
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	drawImageSwitch = true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        default:
            break;
    }
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
