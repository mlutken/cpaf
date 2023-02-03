#ifndef CPAF_CONTAINER_UTILS_H
#define CPAF_CONTAINER_UTILS_H

#include <cstddef>

namespace cpaf::containers
{

std::size_t buffer_read_forward(unsigned char*& cur_pos, const unsigned char* buffer_begin,
                                const size_t buffer_size, const size_t chunk_size);
std::size_t buffer_read_forward(char*& cur_pos, const char* buffer_begin,
                                const size_t buffer_size, const size_t chunk_size);


} //end namespace cpaf::containers


#endif //CPAF_CONTAINER_UTILS_H

