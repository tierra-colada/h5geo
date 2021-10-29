#include <iostream>

#include <h5map/h5mapcontainer.h>

int main() 
{ 
  auto mapContainer = H5MapContainer::create("map_container.h5", h5gt::File::OpenOrCreate);
  return 0; 
}