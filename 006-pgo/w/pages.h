#ifndef W_PAGES
#define W_PAGES

#include <sstream>

std::ostringstream& write_header(std::ostringstream& oss, const std::string& pagename);
int write_footer(int fd, std::ostringstream& oss);
std::string html_escape_str(const char* s);
int open_output(int dfd, const char *node);
int close_output(int fd);
int write_page(int dfd, const char *name, int (*fxn)(int, int));

#endif
