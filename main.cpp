#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

const std::string ascii_buffer = " .:-=+*#%@";

std::string get_suitable_ascii(int density)
{
    return std::string(1, ascii_buffer[density * (ascii_buffer.size() - 1) / 255]);
}

std::string to_ascii(const cv::Mat& gray_frame, const int& cell_size)
{
    std::string output;

    for (int y = 0; y < gray_frame.rows; y += cell_size)
    {
        for (int x = 0; x < gray_frame.cols; x += cell_size)
        {
            // Cell region stands for (cell_size * cell_size) sized image piece
            cv::Mat cell_region = gray_frame(cv::Rect(x, y, cell_size, cell_size));
            // Average Color of cell_size * cell_size pixel.
            cv::Scalar average_color = cv::mean(cell_region);
            // Other indexes such as 1, 2 does not work.
            // Zero index were used for gray value in general.
            int color_density = static_cast<int>(average_color[0]);
            std::string pixelToAscii = get_suitable_ascii(color_density);
            output += pixelToAscii;
        }
        output += "\n";
    }

    return output;
}

int main() {

    // VIDEO CAPTURE
    cv::VideoCapture video_capture("../video_source/video1.mp4");

    if (!video_capture.isOpened()) {
        std::cerr << "ERROR: Video could not be captured properly!\n";
        return -1;
    }

    const int esc_key = 27;
    const int ms = 30;

    const float aspect_ratio = 16.f / 9.f;
    const int x_resolution = 1200;
    const int y_resolution = x_resolution / (aspect_ratio);

    const int alignment_offset = 5;
    const int cell_size = 12;
    const int window_width = x_resolution / cell_size * cell_size; // cell_amount_x * cell_size
    const int window_height = y_resolution / cell_size * cell_size; // cell_amount_y * cell_size

    cv::Mat original_frame;
    cv::Mat resized_frame;

    while (true)
    {
        if (!video_capture.read(original_frame))
        {
            std::cerr << "ERROR: Cannot read the frame from video.\n";
            exit(-1);
        }

        cv::resize(original_frame, resized_frame, cv::Size(window_width, window_height));
        cv::cvtColor(resized_frame, resized_frame, cv::COLOR_BGR2GRAY);

        std::string output_text = to_ascii(resized_frame, cell_size);

        cv::Mat output_image(window_height, window_width, CV_8UC3, cv::Scalar(0, 0, 0));

        int y_offset = 0;
        int x_offset = 0;

        for (char c : output_text)
        {
            if (c != '\n')
            {
                cv::putText(output_image, // Output
                            std::string(1, c), // written text
                            cv::Point(x_offset, y_offset + cell_size - alignment_offset), // Where
                            cv::FONT_HERSHEY_SIMPLEX, // Font type
                            0.3, // Font Size
                            cv::Scalar(255, 255, 255), // Font Color - White
                            1); // Thickness

                x_offset += cell_size;
                continue;
            }

            y_offset += cell_size;
            x_offset = 0;
        }

        // Show output on screen.
        cv::imshow("video2ascii-converter", output_image);

        // CLOSE THE WINDOW
        if (cv::waitKey(ms) == esc_key)
        {
            break;
        }
    }

    return 0;
}
