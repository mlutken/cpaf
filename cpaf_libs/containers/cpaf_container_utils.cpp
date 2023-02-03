
#include "cpaf_container_utils.h"

namespace cpaf::containers
{

/** Move current position forward in buffer returning actual moved size
@param cur_pos Pointer to the current position in your buffer
@param buffer_begin Pointer to the beginning your buffer
@param buffer_size Total size of your buffer
@param chunk_size Max number of bytes to read in one go from your buffer.

@return The number of bytes actually "read" (moved forward).
*/
std::size_t buffer_read_forward(unsigned char*& cur_pos, const unsigned char* buffer_begin,
                                const size_t buffer_size, const size_t chunk_size)
{
    const auto bytes_consumed = static_cast<size_t>(cur_pos - buffer_begin);
    const auto bytes_left = buffer_size - bytes_consumed;
    auto actual_chunk_size = (bytes_left < chunk_size) ? bytes_left : chunk_size;
    cur_pos += actual_chunk_size;
    return actual_chunk_size;
}

/** Move current position forward in buffer returning actual moved size
@param cur_pos Pointer to the current position in your buffer
@param buffer_begin Pointer to the beginning your buffer
@param buffer_size Total size of your buffer
@param chunk_size Max number of bytes to read in one go from your buffer.

@return The number of bytes actually "read" (moved forward).
*/
std::size_t buffer_read_forward(char*& cur_pos, const char* buffer_begin,
                                const size_t buffer_size, const size_t chunk_size)
{
    const auto bytes_consumed = static_cast<size_t>(cur_pos - buffer_begin);
    const auto bytes_left = buffer_size - bytes_consumed;
    auto actual_chunk_size = (bytes_left < chunk_size) ? bytes_left : chunk_size;
    cur_pos += actual_chunk_size;
    return actual_chunk_size;
}


} //end namespace cpaf::containers


