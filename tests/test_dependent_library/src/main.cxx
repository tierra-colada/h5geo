#include <iostream>

#include <h5surf/h5surfcontainer.h>

int main() 
{ 
  auto surfContainer = H5SurfContainer::create("surf_container.h5", h5gt::File::OpenOrCreate);
  return 0; 
}