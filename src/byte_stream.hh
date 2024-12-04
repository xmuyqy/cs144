#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <deque>
class Reader;
class Writer;

class ByteStream
{

public:
  std::deque<char> buffer_ = {};
  size_t capacity_ = 0;
  size_t read_count_ = 0;
  size_t write_count_ = 0;
  bool input_ended_flag_ = false;
  bool error_ = false;
  std::string_view buffer_view_{};
  std::string tmp_{""};
public:
  explicit ByteStream( uint64_t capacity );

  // Helper functions (provided) to access the ByteStream's Reader and Writer interfaces
  Reader& reader();
  const Reader& reader() const;
  Writer& writer();
  const Writer& writer() const;

  bool has_error() const { return error_; }; // Has the stream had an error?

  // for writer
  size_t write(const std::string &data);
  size_t remaining_capacity() const;
  void end_input();
  void set_error();

  // for reader
  std::string peek_output(const size_t len) const; // Remove ''len'' bytes from the buffer
  void pop_output(const size_t len); // Read (i.e., copy and then pop) the next "len" bytes of the stream
  std::string read(const size_t len);
  bool input_ended() const; // 'true' if the stream input has ended
  bool eof() const; // 'true' if the output has reached the ending
  bool error() const; // 'true' if the stream has suffered an error
  size_t buffer_size() const; // the maximum amount that can currently be peeked/read
  bool buffer_empty() const; // 'true' if the buffer is empty
  size_t bytes_written() const; // Total number of bytes written
  size_t bytes_read() const; // Total number of bytes popped

};

class Writer : public ByteStream
{
public:
  void push( std::string data ); // Push data to stream, but only as much as available capacity allows.
  void close();                  // Signal that the stream has reached its ending. Nothing more will be written.

  bool is_closed() const;              // Has the stream been closed?
  uint64_t available_capacity() const; // How many bytes can be pushed to the stream right now?
  uint64_t bytes_pushed() const;       // Total number of bytes cumulatively pushed to the stream
};

class Reader : public ByteStream
{
public:
  std::string_view peek() const; // Peek at the next bytes in the buffer
  void pop( uint64_t len );      // Remove 'len' bytes from the buffer

  bool is_finished() const;        // Is the stream finished (closed and fully popped)?
  uint64_t bytes_buffered() const; // Number of bytes currently buffered (pushed and not popped)
  uint64_t bytes_popped() const;   // Total number of bytes cumulatively popped from stream
};

/*
 * read: A (provided) helper function thats peeks and pops up to 'len' bytes
 * from a ByteStream Reader into a string;
 */
void read( Reader& reader, uint64_t len, std::string& out );
