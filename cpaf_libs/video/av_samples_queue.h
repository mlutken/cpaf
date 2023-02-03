#ifndef CPAF_VIDEO_AV_SAMPLES_QUEUE_H
#define CPAF_VIDEO_AV_SAMPLES_QUEUE_H



#include <utility>
#include <concurrent/srsw_fifo.hpp>
#include <cpaf_libs/video/av_samples_buffer.h>


namespace cpaf::video {
/**
 *  @todo Do we really need to ad functionality to this wrapper at all ?
 *        Not used currently!
 *
 * Lockless fifo of audio av_samples_buffer(s)
 *  */
class av_samples_queue
{
public:

	// --- Constructors etc. ---
	av_samples_queue();
	av_samples_queue (const av_samples_queue&) = delete;

	av_samples_queue& operator=(const av_samples_queue&)  = delete;
	~av_samples_queue();



	size_t					size			() const	{ return samples_queue_.size();}
//	void					flush			()			{ samples_queue_.flush();}


	// --- Info functions ---
	std::string		dbg_string_short	() const;

private:
	using samples_queue_t = estl::srsw_fifo<av_samples_buffer>;

	samples_queue_t		samples_queue_;
};



} //END namespace cpaf::video


#endif //CPAF_VIDEO_AV_SAMPLES_QUEUE_H
