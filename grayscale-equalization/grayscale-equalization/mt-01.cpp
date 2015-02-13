#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


/*
Implementujte vlastnoru�n� funkci, kter� p�evede barevn� 3-kan�lov� obraz ve form�tu BGR do jednokan�lov�ho �edot�nov�ho obrazu.
G = 0.299*R + 0.587*G + 0.114*B;

Upozorn�n�: ...
Note that the default color format in OpenCV is often referred to as RGB but it is actually BGR (the bytes are reversed).
So the first byte in a standard (24-bit) color image will be an 8-bit Blue component, the second byte will be Green, and
the third byte will be Red.

Realizujte pomoc� ukazatel� p��mo do pam�ti: unsigned char * ptr.
Ukazatel na data matice je: Mat.data.
Pozor, ���ka ��dku nemus� z d�vodu zarovn�n� v pam�ti odpov�dat: po�et_kan�l� * ���ka_matice
- tedy ne: Mat.cols*Mat.channels()
- ale vyu��t atribut: Mat.step

*/
void bgr2gray(const cv::Mat bgr, cv::Mat& gray)
{
	gray = cv::Mat::zeros(bgr.rows, bgr.cols, CV_8UC1);

	/*
	Zde bude vlastnoru�n� implementace algoritmu pro p�evod BGR na GRAY obr�zek.
	*/

}



/*  Funkce pro v�po�et histogramu �edot�nov�ho obrazu (0-255) a pomoc� ekvalizace histogramu zlep�en� jeho kontrastu.
M��ete vyu��t funkci pro p��stup k hodnot�m matice: Mat.at<>
P��klad: hodnotu jasu pixelu na ��dku 100 a sloupci 200 (tedy x=200, y=100) �edot�nov�ho obrazu z�sk�me:
unsigned char pixel_value = obraz.at<unsigned chat>(100,200);

D�le se hod� funkce saturate_cast, kter� prov�d� efektivn� a p�esn� p�evod mezi ��seln�mi typy.
P��klad sn�en� kontrastu na polovic a sn�en� intensity jasu cel�ho obrazu o 127 pak m��e pro jeden pixel vypadat takto:
obraz.at<unsigned chat>(100,200) = saturate_cast<unsigned char>( 0.5 * obraz.at<unsigned chat>(100,200) - 127.0 );

Informace o ekvalizaci histogramu, nap�.:
http://docs.opencv.org/doc/tutorials/imgproc/histograms/histogram_equalization/histogram_equalization.html

*/
void histogramEqualization(const cv::Mat gray, cv::Mat& eq_gray)
{
	cv::Mat h = cv::Mat::zeros(1, 256, CV_32SC1);
	eq_gray = cv::Mat::zeros(gray.size(), CV_8UC1);

	// projd�te obraz a spo�t�te histogram obrazu
	// ...


	// spo�t�te akumulovan� histogram
	// hodnota v akumulovan�m histogramu pro danou intensitu je rovna sou�tu v�ech hodnot histogramu s ni���mi intenzitami.
	// ...


	// p�epo�ti hodnoty jas� v obraze
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