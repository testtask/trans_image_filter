
#ifndef __IMG_FILTER_HPP__
#define __IMG_FILTER_HPP__

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <string>

namespace img_filter_ns {

    class img_filter_c {

    public:
        typedef uint8_t data_type_t;

    public:  
        explicit img_filter_c(const std::vector<data_type_t> &buf);

        img_filter_c(const img_filter_c& other);
        img_filter_c(img_filter_c&& other) noexcept;

        img_filter_c& operator=(const img_filter_c& other);
        img_filter_c& operator=(img_filter_c&& other) noexcept;


        virtual ~img_filter_c() noexcept;


        void safe(std::vector<data_type_t> *dst) const;

        void denoize(int32_t kernel_size = 3);
        void detect_edges(int32_t kernel_size = 3);

    private:
        typedef cv::Mat img_t;
        void init(const std::vector<data_type_t> &buf);

        img_t m_img;
        img_t m_tmp;
       
    };

};


#endif //#ifndef __IMG_FILTER_HPP__