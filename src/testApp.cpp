#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
    ofBackground(ofColor::white);
    //    ofSetBackgroundAuto(false);
    
    //image1とimage2は、同じサイズであることを前提とする
    image1.loadImage("toga.jpg");
    image2.loadImage("monalisa.jpg");
    originalImage1 = image1;
		originalImage2 = image2;
		
    drawImageSwitch = false;
    counter = -1;
    for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
        usedOrNotVector.push_back(false);
        exchangeOrderVector.push_back(i);
    }
    random_shuffle(exchangeOrderVector.begin(), exchangeOrderVector.end());
    enableDuplicate = false;
    enableExchange = true;
    enableRandomExchange = true;
		enableMultipixelDrawing = true;
}

//--------------------------------------------------------------
void testApp::update(){
    if (counter <= image1.getWidth()*image1.getHeight()) {
        counter++;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    if (counter <= image1.getWidth()*image1.getHeight()) {
        // 左の画像を右の画像に入れ替える
        ofPoint referencePoint;
        if (enableRandomExchange) {
            //ランダムで交換
            referencePoint = ofPoint(exchangeOrderVector[counter]%(int)image1.getWidth(), exchangeOrderVector[counter]/(int)image1.getWidth());
            
        }else{
            //上から順に交換
            referencePoint = ofPoint(counter%(int)image1.getWidth(), counter/(int)image1.getWidth());
        }
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
        
        // 左の画像の色を入れ替える
        image1.setColor(pointPairVector.back().point1.x, pointPairVector.back().point1.y, image2.getColor(pointPairVector.back().point2.x, pointPairVector.back().point2.y));
        image1.update();
        
        // 右の画像の色を入れ替える
        if (enableExchange) {
            image2.setColor(pointPairVector.back().point2.x, pointPairVector.back().point2.y, originalImage1.getColor(pointPairVector.back().point1.x, pointPairVector.back().point1.y));
            image2.update();
        }
    }
    
    // draw image
    image1.draw(ofPoint(ofGetWidth()/4.-image1.getWidth()/2.+100, (ofGetHeight()-image1.getHeight())/2.), image1.getWidth(), image1.getHeight());
    image2.draw(ofPoint(ofGetWidth()/4.*3-image2.getWidth()/2.-100 , (ofGetHeight()-image2.getHeight())/2.), image2.getWidth(), image2.getHeight());
    
    //hightLihgting
    if (counter <= image1.getWidth()*image1.getHeight()) {
        ofPushStyle();
        ofSetColor(ofColor::white);
        ofSetLineWidth(1);
        ofLine(pointPairVector.back().point1.x+(ofGetWidth()/4.-image1.getWidth()/2.+100), pointPairVector.back().point1.y+((ofGetHeight()-image1.getHeight())/2.),
               pointPairVector.back().point2.x+(ofGetWidth()/4.*3-image2.getWidth()/2.-100), pointPairVector.back().point2.y+((ofGetHeight()-image2.getHeight())/2.));
        ofPopStyle();
    }
		
		ofPushStyle();
				ofSetColor(255, 255, 0, 127);
				ofCircle(300, 100, 150);
				ofSetColor(0, 255, 255, 127);
				ofCircle(100, 100, 150);

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
				case '0':
						ofEnableBlendMode(OF_BLENDMODE_DISABLED);
						break;
				case '1':
						ofEnableBlendMode(OF_BLENDMODE_ALPHA);
						break;
				case '2':
						ofEnableBlendMode(OF_BLENDMODE_ADD);
						break;
				case '3':
						ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
						break;
				case '4':
						ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
						break;
				case '5':
						ofEnableBlendMode(OF_BLENDMODE_SCREEN);
						break;
				case 'a':
						ofEnableAntiAliasing();
						break;
				case 'b':
						ofBackground(0, 0, 0, 255);
						break;
				case 'e':
						ofEnableAlphaBlending();
						break;
				case 'd':
						ofDisableAlphaBlending();
						break;
				case 's':
						ofDisableAntiAliasing();
						break;
				case 'w':
						ofBackground(255, 255, 255, 255);
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
