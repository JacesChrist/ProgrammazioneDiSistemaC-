#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "File.h"

uintmax_t File::getSize () const {
    return this->size;
}

void File::ls (int indent=0) const  {
    std::cout<<"File name "<<this->getName()<<" size "<<this->size;
}
