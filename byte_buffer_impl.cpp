#include "byte_buffer.hpp"




byte_buffer::byte_buffer( ubyte_t *const buffptr   ,
                          const bool     read_only ,
                          const uint16_t capacity  ,
                          const uint16_t offset    ,
                          const uint16_t limit     ,
                          const uint16_t position  )
                        : _buffptr(   buffptr + offset ) ,
                          _read_only( read_only        ) ,
                          _capacity(  capacity         ) ,
                          _limit(     limit            ) ,
                          _position(  position         ) {}




byte_buffer::byte_buffer( ubyte_t *const buffptr   ,
                          const uint16_t capacity  , 
                          const uint16_t offset    ,
                          const bool     read_only )
                        : _buffptr(   buffptr + offset ) ,
                          _read_only( read_only        ) ,
                          _capacity(  capacity         ) ,
                          _limit(     capacity         ) ,
                          _position(  0                ) {}




byte_buffer::byte_buffer( byte_buffer      &copy      ,
                          const uint16_t  capacity  ,
                          const uint16_t  offset    ,
                          const bool      read_only )
                        : _buffptr(   copy._buffptr + offset                      ) ,
                          _read_only( copy._read_only || read_only                ) ,
                          _capacity(  copy._capacity < capacity ?  copy._capacity
                                                                :  capacity       ) ,
                          _limit(     _capacity                                   ) ,
                          _position(  0                                           ) {}




void byte_buffer::clear() {
    
    _position = 0         ;
    _limit    = _capacity ;
    }




void byte_buffer::flip() {

    _limit    = _position ;
    _position = 0         ;
    }




template < typename T >
uint16_t byte_buffer::capacity() const { return _capacity / sizeof( T ) ; }




template < typename T >
uint16_t byte_buffer::limit() const { return _limit / sizeof( T ) ; }




template < typename T >
uint16_t byte_buffer::limit( uint16_t new_limit ) {
    
    new_limit *= sizeof( T ) ;

    if ( new_limit <= _capacity ) {
        _limit = new_limit ;
        return 0 ;
        }
    
    return new_limit - _capacity ;
    }




uint16_t byte_buffer::position() const { return _position ; }




template < typename T >
uint16_t byte_buffer::position( uint16_t new_pos ) {

    new_pos *= sizeof( T ) ;

    if ( new_pos < _limit ) {
        _position = new_pos ;
        return 0 ;  
        }
    
    return new_pos - _limit + 1 ;
    }




template < typename T >
uint16_t byte_buffer::remaining() const { return ( _limit - _position ) / sizeof( T ) ; }




template < typename T >
bool byte_buffer::has_remaining() const { return static_cast< bool >( remaining< T >() ) ; } 




template < typename T >
const T* byte_buffer::getr( const uint16_t n ) {
    
    const uint16_t read_size = n * sizeof( T ) ;

    if ( _position <= _limit - read_size ) {
        
        T *const ptr = reinterpret_cast< T* >( _buffptr + _position ) ;
        _position += read_size ;

        return ptr ;
        }
    
    return nullptr ;
    }




template < typename T >
const T* byte_buffer::geta( const uint16_t pos   ,
                            const uint16_t n     )
         const {
    
    return pos <= _limit - n * sizeof( T ) ?  reinterpret_cast< T* >( _buffptr + pos )
                                           :  nullptr                                  ;
    }




template < typename T >
bool byte_buffer::putr( const T value ) {

    if ( _position <= _limit - sizeof( T )) {
        
        *reinterpret_cast< T* >( _buffptr +_position ) = value ;
        _position += sizeof( T ) ;
        return true ;
        }

    return false ;
    }




template < typename T >
uint16_t byte_buffer::putr( const T *const  values ,
                            const uint16_t  n      ) {
    
    if ( _read_only )  return 0 ;

    const uint16_t write_size = n * sizeof( T ) ;

    if ( _position <= _limit - write_size ) {
        
        for ( uint16_t i = 0 ; i < n ; i         += 1          ,
                                       _position += sizeof( T ))
            *reinterpret_cast< T* >( _buffptr + _position ) = values[i] ;

        return 0 ;
        }
    
    return _position + write_size - _limit ;
    }




template < typename T >
bool byte_buffer::puta( const uint16_t pos   ,
                        const T        value ) {
    
    if ( pos <= _limit - sizeof( T )) {
        
        *reinterpret_cast< T* >( _buffptr + pos ) = value ;
        return true ;
        }
    
    return false ;
    }




template < typename T >
uint16_t byte_buffer::puta( const uint16_t  pos    ,
                            const T *const  values ,
                            const uint16_t  n      ) {
    
    const uint16_t write_size = n * sizeof( T ) ;

    if ( _read_only )  return 0 ;

    if ( pos < _limit - write_size ) {
        
        for ( uint16_t i = 0 ; i < n ; i += 1 )
            *reinterpret_cast< T* >( _buffptr + pos + i * sizeof( T ) ) = values[i] ;

        return 0 ;
        }
    
    return _position + write_size - _limit ;
    }
