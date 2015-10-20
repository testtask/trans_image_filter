#include "img_filter.hpp"

#include "boost/program_options.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <string>
#include <exception>
#include <fstream>
#include <vector>

#define MODE_DENOIZE            0x00000001
#define MODE_EDGE_DETECTION     0x00000002

typedef struct settings_s
{
    std::string src_file_name;
    std::string dst_file_name; 
    int32_t mode;
}settings_t;

void validate(const settings_t &par_set)
{
    using namespace boost::program_options;

    int32_t err = 0;

    if (par_set.dst_file_name.empty())
        err = 1, std::cerr << "validation error: dst file name is empty\n";

    if (par_set.src_file_name.empty())
        err = 1, std::cerr << "validation error: src file name is empty\n";

    int32_t v[] = { MODE_DENOIZE, MODE_EDGE_DETECTION, MODE_DENOIZE | MODE_EDGE_DETECTION };
    if (std::find(std::begin(v), std::end(v), par_set.mode) == std::end(v))
        err = 1, std::cerr << "validation error: unknown mode\n";

    if (err)
        throw validation_error(validation_error::invalid_option);
}


using raw_buf_t = std::vector<img_filter_ns::img_filter_c::data_type_t>;

void write_file(const std::string &dst_file_name, const raw_buf_t &buf)
{
    std::fstream dst(dst_file_name, std::ios::binary | std::ios::out);

    if (0 == dst.is_open())
        throw std::exception((std::string("cannot open ") + dst_file_name).c_str());

    dst.seekg(0, std::ios::beg);

    dst.write((char*)&buf[0], buf.size() * sizeof(buf[0]));
    dst.close();
}

void read_file(const std::string &src_file_name, raw_buf_t &buf)
{
    std::fstream src(src_file_name, std::ios::binary | std::ios::in);

    if (0 == src.is_open())
        throw std::exception((std::string("cannot open ") + src_file_name).c_str());

    std::streamsize file_sz = src.tellg();
    src.seekg(0, std::ios::end);
    file_sz = src.tellg() - file_sz;
    src.seekg(0, std::ios::beg);

    buf.resize((raw_buf_t::size_type)file_sz);
    src.read((char*)&buf[0], file_sz);
    src.close();
}

void test(const std::string &src_file_name, const std::string &dst_file_name, int32_t mode)
{
    raw_buf_t buf;

    std::cout << std::endl;
    std::cout << "read input file\n";

    read_file(src_file_name, buf);

    std::cout << "parse file\n";
    img_filter_ns::img_filter_c filter(buf);

    if (mode & MODE_DENOIZE) {
        std::cout << "denoize image\n";
        filter.denoize();
    }

    if (mode & MODE_EDGE_DETECTION) {
        std::cout << "detect edges\n";
        filter.detect_edges(2);
    }

    std::cout << "get output buffer\n";
    filter.safe(&buf);

    std::cout << "safe in output file\n";
    write_file(dst_file_name, buf);
}



int32_t main(int32_t argc, char **argv)
{
    namespace po = boost::program_options;

    settings_t par_set{};
    po::options_description desc("options");

    try {

        desc.add_options()
            (",i", po::value<std::string>(&par_set.src_file_name)->required(), "input JPEG file")
            (",o", po::value<std::string>(&par_set.dst_file_name)->required(), "output JPEG file")
            (",m", po::value<int32_t>(&par_set.mode)->required(), "mode: 1 - denoize, 2 - edge detection, 3 - both")
            ;
        po::variables_map vm;

        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        validate(par_set);
        test(par_set.src_file_name, par_set.dst_file_name, par_set.mode);

    }
    catch (po::error& e) {
        std::cerr << "\nerror in cmd:\n" << e.what() << "\n" << desc << std::endl;
        std::cerr << "\nUsage:\nsample -i <inputfile> -o <outputfile> -m <mode>\n";
    }
    catch (std::exception& e) {
        std::cerr << "\nerror: " << e.what() << ", exit" << std::endl;
    }

    return 0;
}