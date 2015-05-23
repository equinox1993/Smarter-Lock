//
//  ZxingBridge.cpp
//  Smarter Lock
//
//  Created by Yuwei Huang on 15/5/23.
//  Copyright (c) 2015年 CSE481. All rights reserved.
//

#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/MultiFormatReader.h>

#include <opencv2/imgproc/imgproc.hpp>
#include "ZxingBridge.h"

class CvSource : public zxing::LuminanceSource {
public:
	CvSource(const cv::Mat& m) : zxing::LuminanceSource(m.cols, m.rows), mat(m) {}
	zxing::ArrayRef<char> getRow(int y, zxing::ArrayRef<char> row) const {
		const char* pixelRow = (char*)mat.data + y * mat.step;
		if (!row) {
			row = zxing::ArrayRef<char>(getWidth());
		}
		cv::Mat cvRow = mat.row(y);
		cv::Mat grayRow;
		cv::cvtColor(cvRow, grayRow, CV_BGR2GRAY);
		
		for (int i = 0; i < getWidth(); i++) {
			row[i] = grayRow.data[i];
		}
		
		return row;
	}
	zxing::ArrayRef<char> getMatrix() const {
		cv::Mat gray;
		cv::cvtColor(mat, gray, CV_BGR2GRAY);
		
		return zxing::ArrayRef<char>((char*)gray.data, gray.elemSize()*gray.rows*gray.cols);
	}
private:
	cv::Mat mat;
};

namespace ZxingBridge {
	Ref<LuminanceSource> matToLuminance(const cv::Mat& mat) {
		return Ref<LuminanceSource>(new CvSource(mat));
	}
	
	std::string decode(const cv::Mat& mat) {
		Ref<LuminanceSource> source = matToLuminance(mat);
		Ref<Binarizer> binarizer(new GlobalHistogramBinarizer(source));
		DecodeHints hints(DecodeHints::DEFAULT_HINT);
		Ref<BinaryBitmap> binary(new BinaryBitmap(binarizer));
		Ref<Reader> reader(new MultiFormatReader);
		Ref<Result> res = reader->decode(binary, hints);
		
		return res->getText()->getText();
	}
}