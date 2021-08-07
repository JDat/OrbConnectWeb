#ifndef _MESSAGE_STREAM_H
#define _MESSAGE_STREAM_H

#include <vector>
#include <string>
#include <sstream>

using namespace std;

/**
 * This class wraps a stringstream object and 
 * sets it up with a higher default precision
 * (12 digits)
 */
class MessageStream : public stringstream {
public:
    MessageStream::MessageStream() : stringstream() {
        stringstream::precision(12);
    }

};

/**
 * This class wraps an ostringstream object 
 * (output only) and sets it up with a higher
 * default precision (12 digits)
 */
class MessageOutStream : public ostringstream {
public:
    MessageOutStream::MessageOutStream() : ostringstream() {
        ostringstream::precision(12);
    }

};



#endif