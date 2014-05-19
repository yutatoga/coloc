#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
    ofBackground(ofColor::black);
    //    ofSetBackgroundAuto(false);
    
    //image1とimage2は、同じサイズであることを前提とする
    image1.loadImage("toga_debug2.jpg");
    image2.loadImage("monalisa_debug2.jpg");
    originalImage1 = image1;
    originalImage2 = image2;
    
    drawImageSwitch = false;
    counter = -1;
    for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
        usedOrNotVector.push_back(false);
        exchangeOrderVector.push_back(i);
    }
		// 重複がないピクセル数分のインデックスの乱数をつくる
    random_shuffle(exchangeOrderVector.begin(), exchangeOrderVector.end());
    enableDuplicate = false;
    enableExchange = true;
    enableRandomExchange = false;
    enableMultipixelDrawing = true;
    
    ofSetFullscreen(true);
    
    saveToFile = false;
		inAnimation = false;
		startAnimationFrameNum = -1;
		currentPointVector.assign((int)(image1.getWidth()*image1.getHeight()), 0);
}

//--------------------------------------------------------------
void testApp::update(){
    if (!saveToFile) {
        if (counter <= image1.getWidth()*image1.getHeight()) {
            counter++;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
//    ofLog(OF_LOG_NOTICE, "draw"+ofToString(ofGetFrameNum()));
//    if (!saveToFile) {
//        if (counter <= image1.getWidth()*image1.getHeight()) {
//            // 左の画像を右の画像に入れ替える
//            ofPoint referencePoint;
//            if (enableRandomExchange) {
//                //ランダムで交換
//                referencePoint = ofPoint(exchangeOrderVector[counter]%(int)image1.getWidth(), exchangeOrderVector[counter]/(int)image1.getWidth());
//                
//            }else{
//                //上から順に交換
//                referencePoint = ofPoint(counter%(int)image1.getWidth(), counter/(int)image1.getWidth());
//            }
//            ofColor referenceColor = image1.getColor(referencePoint.x, referencePoint.y);
//            double minimumDistance = image1.getWidth()+image2.getHeight();
//            ofPoint minimumDistancePoint;
//            unsigned char * checkImagePixels = image2.getPixels();
//            // 最小の距離の色を調べる
//            for (int i=0; i<image1.getHeight(); i++) {
//                for (int j=0; j<image1.getWidth(); j++) {
//                    if (!enableDuplicate) {
//                        // 重複を許さない
//                        if (!usedOrNotVector[i*(int)image2.getWidth()+j]) {
//                            ofColor checkColor = ofColor(checkImagePixels[(i*(int)image2.getWidth()+j)*3],
//                                                         checkImagePixels[(i*(int)image2.getWidth()+j)*3+1],
//                                                         checkImagePixels[(i*(int)image2.getWidth()+j)*3+2]);
//                            double checkDistance = getColorDistance(referenceColor, checkColor);
//                            if (checkDistance < minimumDistance) {
//                                if (!usedOrNotVector[i*(int)image2.getWidth()+j]) {
//                                    minimumDistance = checkDistance;
//                                    minimumDistancePoint = ofPoint(j, i);
//                                }
//                                // 同じ色の重複を許す
//                                minimumDistance = checkDistance;
//                                minimumDistancePoint = ofPoint(j, i);
//                            }
//                        }
//                    } else{
//                        ofColor checkColor = ofColor(checkImagePixels[(i*(int)image2.getWidth()+j)*3],
//                                                     checkImagePixels[(i*(int)image2.getWidth()+j)*3+1],
//                                                     checkImagePixels[(i*(int)image2.getWidth()+j)*3+2]);
//                        double checkDistance = getColorDistance(referenceColor, checkColor);
//                        if (checkDistance < minimumDistance) {
//                            if (!usedOrNotVector[i*(int)image2.getWidth()+j]) {
//                                minimumDistance = checkDistance;
//                                minimumDistancePoint = ofPoint(j, i);
//                            }
//                            // 同じ色の重複を許す
//                            minimumDistance = checkDistance;
//                            minimumDistancePoint = ofPoint(j, i);
//                        }
//                    }
//                }
//            }
//            //最も近い距離の色の組み合わせをベクターに保存
//            pointPair tempPointPair;
//            tempPointPair.point1 = referencePoint;
//            tempPointPair.point2 = minimumDistancePoint;
//            
//            // 重複を許さない
//            if (!enableDuplicate) {
//                //再利用しないために、使用したPointを保存
//                usedOrNotVector[minimumDistancePoint.y*(int)image2.getWidth()+minimumDistancePoint.x] = true;
//            }
//            pointPairVector.push_back(tempPointPair);
//            //    ofLog(OF_LOG_NOTICE, "X:"+ofToString(pointPairVector.back().point2.x)+" Y:"+ofToString(pointPairVector.back().point2.y));
//            
//            // 左の画像の色を入れ替える
//            image1.setColor(pointPairVector.back().point1.x, pointPairVector.back().point1.y, image2.getColor(pointPairVector.back().point2.x, pointPairVector.back().point2.y));
//            image1.update();
//            
//            // 右の画像の色を入れ替える
//            if (enableExchange) {
//                image2.setColor(pointPairVector.back().point2.x, pointPairVector.back().point2.y, originalImage1.getColor(pointPairVector.back().point1.x, pointPairVector.back().point1.y));
//                image2.update();
//            }
//        }
//        
//        // draw image
//        image1.draw(ofPoint(ofGetWidth()/4.-image1.getWidth()/2.+100, (ofGetHeight()-image1.getHeight())/2.), image1.getWidth(), image1.getHeight());
//        image2.draw(ofPoint(ofGetWidth()/4.*3-image2.getWidth()/2.-100 , (ofGetHeight()-image2.getHeight())/2.), image2.getWidth(), image2.getHeight());
//        
//        //hightLihgting
//        if (counter <= image1.getWidth()*image1.getHeight()) {
//            ofPushStyle();
//            ofSetColor(ofColor::white);
//            ofSetLineWidth(1);
//            ofLine(pointPairVector.back().point1.x+(ofGetWidth()/4.-image1.getWidth()/2.+100), pointPairVector.back().point1.y+((ofGetHeight()-image1.getHeight())/2.),
//                   pointPairVector.back().point2.x+(ofGetWidth()/4.*3-image2.getWidth()/2.-100), pointPairVector.back().point2.y+((ofGetHeight()-image2.getHeight())/2.));
//            ofPopStyle();
//        }
//        
//    }
		if (inAnimation) {
				// debug - アニメーションせずに入れ替える。(動いてる模様)
//				for (int i=0; i<image1.getHeight(); i++) {
//						for (int j=0; j<image1.getWidth(); j++) {
//								image1.setColor(readPointPairVector[i*(int)image2.getWidth()+j].point1.x, readPointPairVector[i*(int)image2.getWidth()+j].point1.y,
//																image2.getColor(readPointPairVector[i*(int)image2.getWidth()+j].point2.x, readPointPairVector[i*(int)image2.getWidth()+j].point2.y));
//						}
//				}
				
				// x軸,y軸それぞれ1個ずつ動くやりかた。(動いてる模様)
				// リセット
//				image1.setColor(ofColor::black);
//				for (int i=0; i<image1.getHeight(); i++) {
//						for (int j=0; j<image1.getWidth(); j++) {
//								//x軸
//								if (readPointPairVector[i*(int)image2.getWidth()+j].point2.x-currentPointVector[i*(int)image2.getWidth()+j].x > 0) {
//										currentPointVector[i*(int)image2.getWidth()+j].x = currentPointVector[i*(int)image2.getWidth()+j].x+1;
//								}else if(readPointPairVector[i*(int)image2.getWidth()+j].point2.x-currentPointVector[i*(int)image2.getWidth()+j].x < 0){
//										currentPointVector[i*(int)image2.getWidth()+j].x = currentPointVector[i*(int)image2.getWidth()+j].x-1;
//								}
//								//y軸
//								if (readPointPairVector[i*(int)image2.getWidth()+j].point2.y-currentPointVector[i*(int)image2.getWidth()+j].y > 0) {
//										currentPointVector[i*(int)image2.getWidth()+j].y = currentPointVector[i*(int)image2.getWidth()+j].y+1;
//								}else if(readPointPairVector[i*(int)image2.getWidth()+j].point2.y-currentPointVector[i*(int)image2.getWidth()+j].y < 0){
//										currentPointVector[i*(int)image2.getWidth()+j].y = currentPointVector[i*(int)image2.getWidth()+j].y-1;
//								}
//								// バグはここに0が入ること
//								// 0,0 になる
//								// readPointPairVector[i*(int)image2.getWidth()+j].point2.x
//								// readPointPairVector[i*(int)image2.getWidth()+j].point2.y)
//								
//								// 色をぬる
//								image1.setColor(currentPointVector[i*(int)image2.getWidth()+j].x, currentPointVector[i*(int)image2.getWidth()+j].y,
//																image2.getColor(readPointPairVector[i*(int)image2.getWidth()+j].point2.x, readPointPairVector[i*(int)image2.getWidth()+j].point2.y));
//						}
//				}
						
				
				
				// 左右もしくは上下にしか動かないやりかた(動いてる模様)
				image1.setColor(ofColor::black);
				for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
								if (abs(readPointPairVector[i].point2.x-readPointPairVector[i].point1.x) <
										abs(readPointPairVector[i].point2.y-readPointPairVector[i].point1.y)) {
										// x方向が先に移動
										if(readPointPairVector[i].point2.x-currentPointVector[i].x < 0){
												//1ピクセル負の方向に移動
												currentPointVector[i].x = currentPointVector[i].x-1;
										}
										else if (readPointPairVector[i].point2.x-currentPointVector[i].x > 0){
												//1ピクセル正の方向に移動
												currentPointVector[i].x = currentPointVector[i].x+1;
										}else{
												//y方向に移動
												if (readPointPairVector[i].point2.y-currentPointVector[i].y < 0) {
														//1ピクセル負の方向に移動
														currentPointVector[i].y = currentPointVector[i].y-1;
												}else if (readPointPairVector[i].point2.y-currentPointVector[i].y > 0){
														//1ピクセル正の方向に移動
														currentPointVector[i].y = currentPointVector[i].y+1;
												}else{
														//ゴールに到着しているので、なにもしない
												}
										}
								}else{
										// y方向が先に移動
										if (readPointPairVector[i].point2.y-currentPointVector[i].y < 0) {
												//1ピクセル負の方向に移動
												currentPointVector[i].y = currentPointVector[i].y-1;
										}else if (readPointPairVector[i].point2.y-currentPointVector[i].y > 0){
												//1ピクセル正の方向に移動
												currentPointVector[i].y = currentPointVector[i].y+1;
										}else{
												//x方向に移動
												if(readPointPairVector[i].point2.x-currentPointVector[i].x < 0){
														//1ピクセル負の方向に移動
														currentPointVector[i].x = currentPointVector[i].x-1;
												}
												else if (readPointPairVector[i].point2.x-currentPointVector[i].x > 0){
														//1ピクセル正の方向に移動
														currentPointVector[i].x = currentPointVector[i].x+1;
												}else{
														//ゴールに到着しているので何もしない
												}
										}
								}
								//ずらしたpoint1の位置に、point2の色を設定
								image1.setColor(currentPointVector[i].x, currentPointVector[i].y,
																image2.getColor(readPointPairVector[i].point2.x, readPointPairVector[i].point2.y));
				}
				image1.update();
				image1.draw((ofGetWidth()-image1.getWidth())/2.0, (ofGetHeight()-image1.getHeight())/2.0, image1.getWidth(), image2.getHeight());
				image2.draw((ofGetWidth()-image1.getWidth())/2.0, (ofGetHeight()-image1.getHeight())/2.0+200, image1.getWidth(), image2.getHeight());
		}
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
        case 'd':
            ofDisableAlphaBlending();
            break;
        case 'e':
            ofEnableAlphaBlending();
            break;
        case 'r':
            //read file
            importFileForStruct(&readPointPairVector);
            setupCurrentPointVector(readPointPairVector);
						ofLog(OF_LOG_NOTICE, "CHECK:"+ofToString(readPointPairVector.size()));
//            for (int i=0; i<image1.getHeight(); i++) {
//                for (int j=0; j<image1.getWidth(); j++) {
//                    image1.setColor(readPointPairVector[i*(int)image1.getWidth()+j].point1.x,
//                                    readPointPairVector[i*(int)image1.getWidth()+j].point1.y,
//                                    image2.getColor(readPointPairVector[i*(int)image1.getWidth()+j].point2.x,
//                                                    readPointPairVector[i*(int)image1.getWidth()+j].point2.y));
//                }
//            }
						inAnimation = true;
						startAnimationFrameNum = ofGetFrameNum();
            break;
        case 's':
            ofDisableAntiAliasing();
            break;
        case 'w':
            ofBackground(255, 255, 255, 255);
            break;
        case ' ':
        {
            saveToFile = true;
            ofLog(OF_LOG_NOTICE, "calculating...");
            //ペアのデータを作成 -->
            vector<pointPair> pointPairVectorForWriting;
            for (int k=0; k<image1.getWidth()*image1.getHeight(); k++) {
                if (k <= image1.getWidth()*image1.getHeight()) {
                    // 左の画像を右の画像に入れ替える
                    ofPoint referencePoint;
                    if (enableRandomExchange) {
                        //ランダムで交換
                        referencePoint = ofPoint(exchangeOrderVector[k]%(int)image1.getWidth(), exchangeOrderVector[k]/(int)image1.getWidth());
                    }else{
                        //上から順に交換
                        referencePoint = ofPoint(k%(int)image1.getWidth(), k/(int)image1.getWidth());
                    }
										// 距離を調べる元になる色
                    ofColor referenceColor = image1.getColor(referencePoint.x, referencePoint.y);
                    // for文の中で見つかった今のところの最小距離を保存しておく
										double minimumDistance = getColorDistance(ofColor(0, 0, 0), ofColor(255, 255, 255)); // 初期化はgetColorDistance関数で、最大の距離になるもの
                    // for文の中で見つかった今のところの最小点を保存しておく
										ofPoint minimumDistancePoint(1000, 1000);
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
										// 最短距離の色のペアを保存
                    pointPairVectorForWriting.push_back(tempPointPair);
                }
            }
            ofLog(OF_LOG_NOTICE, "pointPairVectorForWriting done!");
            //<-- ペアのデータを作成
            ofLog(OF_LOG_NOTICE, "X1:"+ofToString(pointPairVectorForWriting[0].point1.x)+" Y1:"+ofToString(pointPairVectorForWriting[0].point1.y));
            ofLog(OF_LOG_NOTICE, "X2:"+ofToString(pointPairVectorForWriting[0].point2.x)+" Y2:"+ofToString(pointPairVectorForWriting[0].point2.y));
            exportFileForStruct(pointPairVectorForWriting);
            ofLog(OF_LOG_NOTICE, ofToString("writing file..."));
            saveToFile = false;
            ofLog(OF_LOG_NOTICE, "done");
        }
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

void testApp::exportFile(){
    vector<unsigned int> testvector;
    testvector.push_back(4);
    testvector.push_back(2);
    testvector.push_back(7);
    testvector.push_back(30);
    testvector.push_back(31);
    
    int count = testvector.size();
    ofstream out(ofToDataPath("deleteme/foo_"+ofToString(ofGetYear())+"-"+ofToString(ofGetMonth())+"-"+ofToString(ofGetDay())+"_"+ofToString(ofGetHours())+"-"+ofToString(ofGetMinutes())+"-"+ofToString(ofGetSeconds())+".dat").c_str(),ios::out | ios::binary);
    if (!out) return 1;
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));// out.write((char *) &count, sizeof(count)); // 最初にサイズを保存
    out.write(reinterpret_cast<const char*>(&testvector[0]), testvector.size() * sizeof(int));
    out.close();
}

void testApp::exportFileForStruct(vector<pointPair> ppv){
    int count = ppv.size();
    ofstream out(ofToDataPath("deleteme/foo_"+ofGetTimestampString()+".dat").c_str(),ios::out | ios::binary);
    if (!out) return 1;
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    out.write(reinterpret_cast<const char*>(&ppv[0]), ppv.size() * sizeof(pointPair));
    out.close();
}

void testApp::importFile(){
    vector<int> testvector;
    int count = 0;
    ifstream in(ofToDataPath("deleteme/foo_2014-05-20-06-20-38-095.dat").c_str(), ios::in | ios::binary);
    if ( !in ) return 1;
    in.read(reinterpret_cast<char*>(&count), sizeof(count)); //countのサイズ分をcountに入れる(一個目のデータがサイズであることが前提)
    testvector.assign(count, 0); //testvectorをcountの数個だけ、0で初期設定する
    in.read(reinterpret_cast<char*>(&testvector[0]), testvector.size()*sizeof(int)); //ベクタの最初の位置を指定, intのサイズ分*ベクタの大きさ
    in.close();
    for ( int i = 0; i < testvector.size(); ++i ){
        cout << testvector[i] << endl;
    }
}

void testApp::importFileForStruct(vector<pointPair> *ppv){
    ppv->clear();
    int count = 0;
    ifstream in(ofToDataPath("deleteme/foo_2014-05-20-06-20-38-095.dat").c_str(), ios::in | ios::binary);
    if ( !in ) return ppv;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    pointPair pp;
    pp.point1 = ofPoint(0, 0);
    pp.point2 = ofPoint(0, 0);
    ppv->assign(count, pp);
    in.read(reinterpret_cast<char*>(&ppv->front()), ppv->size()*sizeof(pointPair));
    ofLog(OF_LOG_NOTICE, "reading done!"+ofToString(ppv->size()));
    in.close();
}

void testApp::setupCurrentPointVector(vector<pointPair> ppv){
		currentPointVector.clear();
		for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
				currentPointVector[i] = ppv[i].point1;
		}
}

void testApp::moveUpDownLeftRight(){
		
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
