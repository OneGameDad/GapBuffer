# GapBuffer

GapBuffer is my version of the gap buffer data structure, also known as a text buffer. 

This was created for the speedy handling of input and editing in a text buffer. In my opinion it is smarter than using a vector of strings in a text editor, as it automatically wraps text as all of the data is contained within a single structure and not across multiple. Without a doubt this could use additional improvements, and I will likely make them in time. For now, here is a working version.

Gap Buffers were developed to minimize the number of copy and/or move operations a computer must perform, as they are slow and thus costly. Their design takes into account how humans interact with a text editor, as they are often inserting and/or removing text at a certain point for a while. Humans usually don't jump around the buffer doing many small operations quickly.

## Possible Future Features

The growth of both the gap and buffer is currently fixed. These could be smarter in terms of the size they grow to and how often they have to be resized. All in order to mimimize the number of times bytes are moved around as that's the most costly operation with a gap buffer.

## Installation

Download the repository and use the following in your project:
```c++
#include <GapBuffer.hpp>
```
## Testing

The main file included in this repository is there so you can see the gap buffer in operation. To run:
```bash
make
./gap_buffer
```

## Contributions

Suggestions are welcome, but as this is a personal project there is no guarantee they will be taken. 

## License

[MIT] (https://choosealicense.com/licenses/mit/)
