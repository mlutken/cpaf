// --- Wx and php has some clashes on Windows. Seems to be mostly php's fault ---
#if (defined _WINDOWS) || (defined WINDOWS) || (defined _WIN32) 
#	undef M_PI
#	undef _WIN32_WINNT
#endif

