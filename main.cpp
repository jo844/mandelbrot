#include "Timer.h"
#include "Mandelbrot.h"

// https://github.com/bshoshany/thread-pool
#include "BS_thread_pool.hpp"

// Image settings
const int MAX_ITERATIONS = 2000, IMAGES = 40, IM_WIDTH = 3840, IM_HEIGHT = 2160;

// Choose palette from Palette.h
std::vector<RGBColor> P1 = bright();

std::complex<double> mid(-0.22815549365396181817563724969947809702434640535346752392651006864119738497484478504906341068553091449597520721798098908960188304035681997402213905544823697673245, -1.1151425080399373604907386106318819796276382139857856901977443296527567697322686573368236673401296970841349717561259777992980411457478958193154209253581064586092942919127973015681128961);
// std::complex<double> mid(-0.743643887035763, 0.13182590421259918);
// std::complex<double> mid(0.369299653,-0.093222884);
// std::complex<double> mid(-0.59990625, 0.4290703125);

int main(void)
{
    std::cout << "Generating " << IMAGES << " frames. Resolution " << IM_WIDTH << "x" << IM_HEIGHT << ".\n";
    std::cout << "Using " << std::thread::hardware_concurrency() << " hardware threads.\n";
    BS::thread_pool pool;

    for (int i = 0; i < IMAGES; i++) {
        pool.push_task(zoom, i);
    }

    pool.wait_for_tasks();
	
    return 0;
}

void zoom(int i) {
    Timer t(i);

    double scale = 4.5 * pow(1.025, 1-i);
    int x, y;
    RGBColor col;

    cv::String filename = cv::format("./img/zoom%d.png", i);

    // Create black canvas
    cv::Mat image (IM_HEIGHT, IM_WIDTH, CV_8UC3, cv::Scalar(0, 0, 0));

    // start = std::chrono::system_clock::now();
    for (x = 0; x < IM_WIDTH; x++) {
        for (y = 0; y < IM_HEIGHT; y++) {

            double x0 = (double(x) / (IM_WIDTH - 1) - 0.5) * scale;
            double y0 = (double(y) / (IM_HEIGHT - 1) - 0.5) * scale / 16 * 9;

            std::complex<double> c(x0 + real(mid), y0 + imag(mid));

            col = mandelbrot(c);
            plot(image, x, y, col);
        }
    }

    cv::imwrite(filename, image);
}

void plot(cv::Mat &image, int x, int y, RGBColor c) {
    // OpenCV uses BGR instead of RGB
    image.at<cv::Vec3b>(y,x)[0] = c.b;
    image.at<cv::Vec3b>(y,x)[1] = c.g;
    image.at<cv::Vec3b>(y,x)[2] = c.r;
}

RGBColor mandelbrot(std::complex<double> c) {
	std::complex<double> z = { 0, 0 };
	int i = 0;
	while (std::abs(z) <= (1 << 16) && i < MAX_ITERATIONS)
	{
		z = z * z + c;
		i++;
	}

	if (i < MAX_ITERATIONS) {
		double log_zn = log10(std::abs(z)) / 2;
		double nu = log10(log_zn / log10(2)) / log10(2);
		double frac = fmod(i + 1 - nu, 1);
		i = i + 1 - nu;

		int ind = i % P1.size();
		int ind2 = (i + 1) % P1.size();

		RGBColor color1 = P1[ind];
		RGBColor color2 = P1[ind2];
		RGBColor color = interpolate_color(color1, color2, frac);
		return color;
	}

	return {0,0,0};
}

RGBColor interpolate_color(RGBColor c1, RGBColor c2, double t) {
	int new_red = lerp(c1.r, c2.r, t);
	int new_green = lerp(c1.g, c2.g, t);
	int new_blue = lerp(c1.b, c2.b, t);
	return { new_red, new_green, new_blue };
}

// Linear interpolation
int lerp(int a, int b, double t) {
	return a + (b - a) * t;
}
