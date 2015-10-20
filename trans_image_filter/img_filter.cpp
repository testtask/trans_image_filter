

#include "img_filter.hpp"

#include <string>
#include <fstream>

namespace img_filter_ns {

    img_filter_c::img_filter_c(const std::vector<data_type_t> &buf) 
    { 
        init(buf); 
    }

    img_filter_c::img_filter_c(const img_filter_c& other)
    {
        m_img = other.m_img.clone();
        m_tmp = other.m_tmp.clone();
    }

    img_filter_c::img_filter_c(img_filter_c&& other) noexcept
    {
        m_img = other.m_img;
        m_tmp = other.m_tmp;

        other.m_img.release();
        other.m_tmp.release();
    }

    img_filter_c& img_filter_c::operator=(const img_filter_c& other)
    {
        if (&other == this)
            return *this;

        m_img = other.m_img.clone();
        m_tmp = other.m_tmp.clone();

        return *this;
    }


    img_filter_c& img_filter_c::operator=(img_filter_c&& other) noexcept
    {
        m_img = other.m_img;
        m_tmp = other.m_tmp;

        other.m_img.release();
        other.m_tmp.release();

        return *this;
    }


    img_filter_c::~img_filter_c() noexcept
    {}

  
    void img_filter_c::init(const std::vector<data_type_t> &buf)
    {
        assert(buf.size());

        cv::InputArray arr(buf);
        m_img = cv::imdecode(arr, CV_LOAD_IMAGE_COLOR);

        if (m_img.empty())
            throw std::runtime_error("cannot decode buffer, not a valid JPEG bitstream");

        cv::cvtColor(m_img, m_tmp, CV_BGR2GRAY);
    }

    void img_filter_c::denoize(int32_t kernel_size)
    {
        assert(0 == m_img.empty());
        assert(3 <= kernel_size);

        kernel_size += !(kernel_size & 1); //should be odd
        cv::medianBlur(m_img, m_img, kernel_size); // use median filter for salt-and-pepper noise
    }

    void img_filter_c::detect_edges(int32_t kernel_size)
    {
        assert(0 == m_img.empty());
        assert(0 == m_tmp.empty());

        double otsu_thresh = cv::threshold(m_tmp, m_tmp, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); //calc threshold for Canny edge detector

        kernel_size += !(kernel_size & 1); //should be odd
        cv::Canny(m_img, m_img, 0.5 * otsu_thresh, otsu_thresh, kernel_size);
    }

    void img_filter_c::safe(std::vector<data_type_t> *dst) const
    {
        assert(dst);
        assert(dst->size());

        if (m_img.empty())
            throw std::runtime_error("image buffer is corrupted");

        const std::string format = ".jpeg";

        cv::imencode(format, m_img, *dst);
    }
};