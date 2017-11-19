# Byte buffers

Need to safely read or write different types to a byte array ?

No need for manually typecasting everything everywhere.

This lightweight C++ library provides an interface akin to java
[ByteBuffers](https://docs.oracle.com/javase/8/docs/api/java/nio/ByteBuffer.html)
and aims to be efficient enough for microcontrollers such as the Arduino
family (C++11).

It is useful for a wide variety of use cases such as handling a binary protocol
over the serial port.

## Usage

See `byte_buffer.hpp` for comprehensive documentation.

```cpp
#include <iostream>
#include "byte_buffer.hpp"


/* Given T, any type.
 *
 * A byte buffer has a capacity of T values.
 *
 * It keeps track of the current position in bytes.
 *
 * It has a limit of T values <= to the capacity.
 *
 * The limit can be set by the user and is useful for limiting reads and
 * writes.
 */


// The array we will wrap.
ubyte_t main_arr[64] ;


int main() {

    // Let's build our byte buffer
    byte_buffer b( main_arr, 64 ) ;

    // Now we add different types of value 'relatively', from the current position
    b.putr< int   >( 42 ) ;
    b.putr< char  >( 'a' ) ;
    b.putr< float >( 42.42 ) ;

    // `flip` sets the limit to the current position and the position to 0.
    // It is typically used for reading. 
    b.flip() ;

    // We can read what we have written using `getr`.
    std::cout << "int : "       << *b.getr< int >()   << std::endl ;
    std::cout << "char : "      << *b.getr< char >()  << std::endl ;
    std::cout << "float : "     << *b.getr< float >() << std::endl ;

    // We have hit the limit, there are no more bytes to read.
    std::cout << "remaining : " << b.remaining<>()    << std::endl ;

    // `clear` resets the position to 0 and the limit to the capacity.
    // It is typically used for writing again.
    b.clear() ;

    // Do whatever you want.

    // Maybe you need to set the position to 10 ints from the start ?
    b.position< int >( 10 ) ;

    // It worked.
    std::cout << "New position in bytes = " << b.position() << std::endl ;

    // Set the limit to 12 ints from the start to make sure we can write only
    // 2 ints from the current position (10 ints).
    b.limit< int >( 12 ) ;

    // Let's copy values from an array.
    int arr[] = { 1, 2, 3 } ;

    // We should check the result, 0 if okay or the number of bytes we are
    // over the limit in case of failure.
    uint16_t res = b.putr( arr, 3 ) ;

    // Actually we are over the limit !
    std::cout << "We are short of : " << res << " bytes" << std::endl ;

    // Sometimes we need to read/write using an absolute position.
    // We can use `geta` and `puta`, akin to `getr` and `putr`.
    char cool[] = "Awesome !" ;

    b.puta< char >( 20, cool, sizeof( cool ) );

    std::cout << b.geta< char >( 20 ) << std::endl ;

    // Let's build another byte buffer, a read only view starting at 10 bytes
    // from the existing one.
    byte_buffer b2( b, 10, true ) ;

    // You should have a pretty good understanding of how it works !

    return 0 ;
    }
```

## License

The MIT License (MIT)
Copyright © 2017 Adam Helinski

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
