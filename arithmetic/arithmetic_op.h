#ifndef SP_TOOLKIT_ARITHMETIC_
#define SP_TOOLKIT_ARITHMETIC_

#include <stdint.h>
#include <string>

int64_t arithmetic(const std::string& instr, 
                   std::string *dbg_str=NULL, 
                   std::string *err_str=NULL);

#endif /* SP_TOOLKIT_ARITHMETIC_ */
