#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  return input_ended_flag_;
}

void Writer::push( string data )
{
  if (input_ended_flag_) {
    return;
  }
  uint64_t length = min(data.size(), available_capacity());
  if (length == 0) {
    return;
  }
  if (data.size() > length) {
    data.resize(length);
  }
  
  buffer_.push_back(std::move(data));
  bytes_pushed_ += length;
  if (buffer_.size() == 1) {
    buffer_view_ = buffer_.front();
  }
  
}

void Writer::close()
{
  input_ended_flag_ = true;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - bytes_pushed_ + bytes_poped_;
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  return input_ended_flag_ && (bytes_buffered() == 0);
}

uint64_t Reader::bytes_popped() const
{
  return bytes_poped_;
}

string_view Reader::peek() const
{
  return buffer_view_;
}

void Reader::pop( uint64_t len )
{
  if (len > bytes_buffered()) {
    return;
  }

  bytes_poped_ += len;
  while (len > 0) {
    if (len >= buffer_view_.size()) { // ???
      len -= buffer_view_.size();
      buffer_.pop_front();
      buffer_view_ = buffer_.front(); // ???
    }
    else {
      buffer_view_.remove_prefix(len);
      return;
    }
  }
}

uint64_t Reader::bytes_buffered() const
{
  return bytes_pushed_ - bytes_poped_;
}
