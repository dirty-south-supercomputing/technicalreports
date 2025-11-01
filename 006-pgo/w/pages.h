#ifndef W_PAGES
#define W_PAGES

#include <sstream>

std::ostringstream& write_header(std::ostringstream& oss, const std::string& pagename);
int write_footer(int fd, std::ostringstream& oss);
std::string html_escape_str(const char* s);

#endif
