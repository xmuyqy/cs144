#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

size_t ByteStream::write(const std::string &data)
{
  if(input_ended_flag_) {
    return 0;
  }
  size_t length = min(data.size(), capacity_ - buffer_size());
  if (length == 0) {
    return 0;
  }
  buffer_.insert(buffer_.end(), data.begin(), data.begin() + length);
  write_count_ += length;

  if (buffer_.size() == length) {
    tmp_ = "";
    tmp_.push_back(buffer_.front());
    buffer_view_ = tmp_;
  }
  return length;
}

size_t ByteStream::remaining_capacity() const
{
  return capacity_ - buffer_.size();
}

void ByteStream::end_input()
{
  input_ended_flag_ = true;
}

void ByteStream::set_error()
{
  error_ = true;
}

std::string ByteStream::peek_output(const size_t len) const
{
  if(input_ended_flag_) {
    return 0;
  }
  size_t length = min(len, buffer_.size());
  return string(buffer_.begin(), buffer_.begin() + length);
}

void ByteStream::pop_output(const size_t len)
{
  size_t length = min(len, buffer_.size());
  buffer_.erase(buffer_.begin(), buffer_.begin() + length);
  read_count_ += length;

  if (len < buffer_view_.size()) {
    buffer_view_.remove_prefix(len);
  }
  else {
    tmp_ = "";
    if (!buffer_.empty()) {
      tmp_.push_back(buffer_.front());
    }
    buffer_view_ = tmp_;
  }
}

std::string ByteStream::read(const size_t len)
{
  if (len == 0) {
    return "";
  }
  string str = peek_output(len);
  pop_output(len);
  return str;
}

bool ByteStream::input_ended() const
{
  return input_ended_flag_;
}

bool ByteStream::eof() const
{
  return input_ended() && buffer_empty();
}

bool ByteStream::error() const
{
  return error_;
}

size_t ByteStream::buffer_size() const
{
  return buffer_.size();
}

bool ByteStream::buffer_empty() const
{
  return buffer_.empty();
}

size_t ByteStream::bytes_written() const
{
  return write_count_;
}

size_t ByteStream::bytes_read() const
{
  return read_count_;
}




bool Writer::is_closed() const
{
  return input_ended_flag_;
}

void Writer::push( string data )
{
  write(data);
}

void Writer::close()
{
  end_input();
}

uint64_t Writer::available_capacity() const
{
  return remaining_capacity();
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_written();
}

bool Reader::is_finished() const
{
  return eof();
}

uint64_t Reader::bytes_popped() const
{
  return bytes_read();
}

string_view Reader::peek() const
{
  return buffer_view_;
}

void Reader::pop( uint64_t len )
{
  pop_output(len);
}

uint64_t Reader::bytes_buffered() const
{
  return buffer_size();
}
