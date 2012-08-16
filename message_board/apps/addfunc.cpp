#include "addfunc.h"

namespace apps {

void filedetect(std::string& path , std::string& path_thumb , std::string& filetype){
	if(filetype == "image/png") { path="png"; path_thumb="png"; }
	if(filetype == "image/jpeg") { path="jpg"; path_thumb="jpg"; }
	if(filetype == "image/gif") { path="gif"; path_thumb="gif"; }
	if(filetype == "image/svg+xml") path="svg";
	if(filetype == "audio/mpeg") path="mp3";
	if(filetype == "audio/ogg") path="ogg";
	if(filetype == "audio/vnd.wave") path="wav";
	if(filetype == "text/css") path="css";
	if(filetype == "text/csv") path="csv";
	if(filetype == "text/html") path="html";
	if(filetype == "text/javascript") path="js";
	if(filetype == "text/xml") path="xml";
	if(filetype == "video/mpeg") path="mpg";
	if(filetype == "video/mp4") path="mp4";
	if(filetype == "video/ogg") path="ogg";
	if(filetype == "video/webm") path="webm";
	if(filetype == "video/x-matroska") path="mkv";
	if(filetype == "video/x-ms-wmv") path="wmv";
	if(filetype == "video/x-flv") path="flv";
	if(filetype == "application/pdf") path="pdf";
	if(filetype == "application/zip") path="zip";
	if(filetype == "application/x-gzip") path="gz";
	if(filetype == "application/x-tar") path="tar";
	if(filetype == "application/x-deb") path="deb";
	if(filetype == "application/x-rar") path="rar";
}

} // namespace apps
