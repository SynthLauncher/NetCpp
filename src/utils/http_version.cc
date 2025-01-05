#include "include/utils/http_version.hh"

std::string httpVersionToString(HttpVersion version){
  switch (version) {
    case HttpVersion::HTTP1:
      return "HTTP/1.1";
    case HttpVersion::HTTP2:
      return "HTTP/2.0";
    case HttpVersion::HTTP3:
      return "HTTP/3.0";
    default:
    // TODO: HANDLE ERROR
    return "HTTP/1.1";
  }
}

HttpVersion stringToHttpVersion(std::string version){
  if(version == "HTTP/1.1"){
    return HttpVersion::HTTP1;
  }else if(version == "HTTP/2.0"){
    return HttpVersion::HTTP2;
  }else if(version == "HTTP/3.0"){
    return HttpVersion::HTTP3;
  }else{
    // TODO: HANDLE ERROR
    return HttpVersion::HTTP1;
  }
}
