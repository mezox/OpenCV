#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


/*
Implementujte vlastnoruènì funkci, která pøevede barevný 3-kanálový obraz ve formátu BGR do jednokanálového šedotónového obrazu.
G = 0.299*R + 0.587*G + 0.114*B;

Upozornìní: ...
Note that the default color format in OpenCV is often referred to as RGB but it is actually BGR (the bytes are reversed).
So the first byte in a standard (24-bit) color image will be an 8-bit Blue component, the second byte will be Green, and
the third byte will be Red.

Realizujte pomocí ukazatelù pøímo do pamìti: unsigned char * ptr.
Ukazatel na data matice je: Mat.data.
Pozor, šíøka øádku nemusí z dùvodu zarovnání v pamìti odpovídat: poèet_kanálù * šíøka_matice
- tedy ne: Mat.cols*Mat.channels()
- ale využít atribut: Mat.step

*/
void bgr2gray(const cv::Mat bgr, cv::Mat& gray)
{
	gray = cv::Mat::zeros(bgr.rows, bgr.cols, CV_8UC1);

	/*
	Zde bude vlastnoruèní implementace algoritmu pro pøevod BGR na GRAY obrázek.
	*/

}



/*  Funkce pro výpoèet histogramu šedotónového obrazu (0-255) a pomocí ekvalizace histogramu zlepšení jeho kontrastu.
Mùžete využít funkci pro pøístup k hodnotám matice: Mat.at<>
Pøíklad: hodnotu jasu pixelu na øádku 100 a sloupci 200 (tedy x=200, y=100) šedotónového obrazu získáme:
unsigned char pixel_value = obraz.at<unsigned chat>(100,200);

Dále se hodí funkce saturate_cast, která provádí efektivní a pøesný pøevod mezi èíselnými typy.
Pøíklad snížení kontrastu na polovic a snížení intensity jasu celého obrazu o 127 pak mùže pro jeden pixel vypadat takto:
obraz.at<unsigned chat>(100,200) = saturate_cast<unsigned char>( 0.5 * obraz.at<unsigned chat>(100,200) - 127.0 );

Informace o ekvalizaci histogramu, napø.:
http://docs.opencv.org/doc/tutorials/imgproc/histograms/histogram_equalization/histogram_equalization.html

*/
void histogramEqualization(const cv::Mat gray, cv::Mat& eq_gray)
{
	cv::Mat h = cv::Mat::zeros(1, 256, CV_32SC1);
	eq_gray = cv::Mat::zeros(gray.size(), CV_8UC1);

	// projdìte obraz a spoètìte histogram obrazu
	// ...


	// spoètìte akumulovaný histogram
	// hodnota v akumulovaném histogramu pro danou intensitu je rovna souètu všech hodnot histogramu s nižšími intenzitami.
	// ...


	// pøepoèti hodnoty jasù v obraze
	//double norm_coef = 255./(gray.cols*gray.rows);
	// ...

	return;
}



//---------------------------------------------------------------------------
void checkDifferences(const cv::Mat test, const cv::Mat ref, std::string tag, bool save = false);
//---------------------------------------------------------------------------


//
// Examples of input parameters
//
// ./mt-01 ../data/garden.png 


int main(int argc, char* argv[])
{
	std::string img_path = "";

	// check input parameters
	if (argc > 1) {
		img_path = std::string(argv[1]);
	}

	// load testing images
	cv::Mat src_rgb = cv::imread(img_path);

	// check testing images
	if (src_rgb.empty()) {
		std::cout << "Failed to load image: " << img_path << std::endl;
		return -1;
	}


	// prepare reference solution
	cv::Mat gray_ref, equa_ref;
	cv::cvtColor(src_rgb, gray_ref, CV_BGR2GRAY);
	cv::equalizeHist(gray_ref, equa_ref);


	// execute implemented function
	cv::Mat gray_man, equa_man;
	bgr2gray(src_rgb, gray_man);
	// using reference gray image		
	histogramEqualization(gray_ref, equa_man);


	// evaluate solution
	checkDifferences(gray_man, gray_ref, "01_bgr2gray", true);
	checkDifferences(equa_man, equa_ref, "01_hist_equ", true);
	std::cout << std::endl;

	return 0;
}
//---------------------------------------------------------------------------



void checkDifferences(const cv::Mat test, const cv::Mat ref, std::string tag, bool save)
{
	double mav = 255., err = 255., nonzeros = 255.;

	if (!test.empty()) {
		cv::Mat diff;
		cv::absdiff(test, ref, diff);
		cv::minMaxLoc(diff, NULL, &mav);
		err = (cv::sum(diff).val[0] / (diff.rows*diff.cols));
		nonzeros = 1. * cv::countNonZero(diff) / (diff.rows*diff.cols);

		if (save) {
			diff *= 255;
			cv::imwrite((tag + ".png").c_str(), test);
			cv::imwrite((tag + "_err.png").c_str(), diff);
		}
	}

	printf("%s %.2f %.2f %.2f ", tag.c_str(), err, nonzeros, mav);

	return;
}