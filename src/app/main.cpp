#include "Bson/bson.h"

#include <iostream>
#include <iterator>
#include <chrono>

#include <boost/filesystem.hpp>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return -1;
    }

    boost::filesystem::path filePath = argv[1];
    assert(!filePath.empty());

    if (!boost::filesystem::exists(filePath))
    {
        std::cerr << "File " << filePath << " doesn't exist\n";
        return -1;
    }

    if (!boost::filesystem::is_regular_file(filePath))
    {
        std::cerr << "File " << filePath << " is not a regular file\n";
        return -1;
    }

    const auto fileSize = boost::filesystem::file_size(filePath);

    boost::filesystem::ifstream fstream(filePath, std::ios_base::binary);
    assert(fstream);
    fstream.unsetf(std::ios::skipws);

    Bson::Bytes content;
    content.reserve(fileSize);
    content.insert(content.begin(), std::istream_iterator<Bson::Byte>(fstream), std::istream_iterator<Bson::Byte>());
    assert(content.size() == fileSize);

    std::cout << "Content size: " << content.size() << "\n";

    const auto time1 = std::chrono::high_resolution_clock::now();

    Bson::Bytes result = Bson::encode(Bson::decode(content));

    const auto time2 = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( time2 - time1 ).count();

    std::cout << "Result: " << std::boolalpha << (result == content) << "\n";
    std::cout << "Duration: " << duration << "\n";

    return 0;
}
