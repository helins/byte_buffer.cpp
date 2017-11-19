#ifndef BYTE_BUFFER_HPP

#define BYTE_BUFFER_HPP


#include <stdint.h>
#include <stddef.h>




using byte_t    = int8_t ;

using ubyte_t   = uint8_t ;

using float32_t = float ;




class byte_buffer {

    private:    

        ubyte_t *const
            _buffptr ;    ///< Pointer to the start of the buffer.
        
        const bool
            _read_only ;  ///< Is this buffer read only ?

        const uint16_t
            _capacity ;   ///< The number of ubytes this buffer can hold at most.

        uint16_t
            _position ,   ///< The current position in the buffer.
            _limit    ;   ///< The current maximum position for reads and writes.

        
        /**
         * Private constructor for building a buffer in details.
         *
         * @param buffptr   Pointer to a ubyte array.
         * @param read_only Is this buffer read only ?
         * @param capacity  Capacity of this buffer.
         *                  Must be within the array bounds.
         * @param offset    Start index for the given array.
         *                  Must be < the capacity.
         * @param limit     Sets the current limit.
         *                  Must be <= the capacity.
         * @param position  Sets current position.
         *                  Must be < the limit.
         */
        byte_buffer( ubyte_t *const buffptr   ,
                     const bool     read_only ,
                     const uint16_t capacity  ,
                     const uint16_t offset    ,
                     const uint16_t limit     ,
                     const uint16_t position  ) ;


    public:
        
        /**
         * Public constructor for building a buffer from an array.
         *
         * @param buffptr   Pointer to a ubyte array.
         * @param capacity  Capacity of this buffer.
         *                  Must be within the array bounds.
         * @param offset    Start index for the given array.
         *                  Defaults to 0.
         * @param read_only Is this buffer read only ?
         *                  Defaults to false.
         */
        byte_buffer( ubyte_t *const buffptr            ,
                     const uint16_t capacity           , 
                     const uint16_t offset    = 0      ,
                     const bool     read_only = false  ) ;
        

        /**
         * Public constructor for building a buffer from another buffer.
         *
         * @param copy      Reference to the original buffer.
         * @param capacity  Capacity of this buffer.
         *                  Capped to the capacity of the original buffer.
         * @param offset    Offset from the start index of the original buffer.
         *                  Default to 0.
         * @param read_only Is this buffer read only ?
         *                  Defaults to false.
         */
        byte_buffer( byte_buffer    &copy              ,
                     const uint16_t  capacity          ,
                     const uint16_t  offset    = 0     ,
                     const bool      read_only = false ) ;
        

        /**
         * Resets the position to 0 and the limit to the capacity.
         * Typically used for new writes.
         */ 
        void clear() ;
        

        /**
         * Sets the limit to the current position and the position to 0.
         * Typically used for reads.
         */
        void flip() ;
        

        /**
         * Gets the capacity of this buffer.
         *
         * @tparam T ubyte_t by default.
         *
         * @return The number of T this buffer can hold at most.
         */
        template < typename T = ubyte_t >
        uint16_t capacity() const ;
        

        /**
         * Gets the current limit of this buffer.
         *
         * @tparam T ubyte_t by default.
         *
         * @return The number of T this buffer can hold before hitting the current limit.
         */
        template < typename T = ubyte_t >
        uint16_t limit() const ;
        

        /**
         * Sets the current limit to a new limit.
         *
         * @tparam T         ubyte_t by default.
         *
         * @param  new_limit The new limit in T for this buffer.
         *                   Must be <= the capacity in T.
         *
         * @return 0         if new_limit <= capacity in T.
         *         n > 0     how many ubyte_t the new limit is over the capacity.
         */
        template < typename T = ubyte_t >
        uint16_t limit( uint16_t new_limit ) ;


        /**
         * Gets the current position of this buffer.
         *
         * @return The current position in ubytes.
         */
        uint16_t position() const ;
        

        /**
         * Sets the current position to a new position.
         *
         * @tparam T       ubyte_t by default.
         *
         * @param  new_pos The new position in T for this buffer.
         *                 Must be <= current limit in T.
         *          
         * @return 0       if new_pos <= current limit in T.
         *         n > 0   how many ubyte_t the new position is over the current limit.
         */
        template < typename T = ubyte_t >
        uint16_t position( uint16_t new_pos ) ;

        
        /**
         * Check how many T can be read or written before hitting the limit.
         *
         * @tparam T ubyte_t by default.
         *
         * @return The number of T till the limit is reached.
         */
        template < typename T = ubyte_t >
        uint16_t remaining() const ;


        /**
         * Checks if at least one T can be read or written before hitting the limit.
         *
         * @tparam T ubyte_t by default.
         *
         * @return True if one T can be put before hitting the limit.
         */
        template < typename T = ubyte_t>
        bool has_remaining() const ;


        /**
         * Gets a pointer to `n` x T values to be read relatively to the current position.
         * Advances position to the next byte after what is requested.
         * Enforces bound checking.
         *
         * @tparam T       ubyte_t by default.
         *
         * @param  n       The number of T the requested pointer is supposed.
         *                 to point to.
         *                 Defaults to 1.
         *
         * @return Tptr    if safe.
         *         nullptr if n overstep the current limit.
         */
        template < typename T = ubyte_t >
        const T* getr( const uint16_t n = 1 ) ;


        /**
         * Gets a pointer to `n` * T values to be read from an absolute position.
         * Does not affect the current position.
         * Enforces bound checking.
         *
         * @tparam T       ubyte_t by default.
         *
         * @param  pos     The absolute starting position.
         * @param  n       How many T will be read.
         * 
         * @return Tptr    if safe.
         *         nullptr if n overstep the current limit.
         */
        template < typename T = ubyte_t >
        const T* geta( const uint16_t pos     ,
                       const uint16_t n   = 1 )
                 const ;

        
        /**
         * Puts a T value relatively to the current position.
         * Advances position to the next byte after what is written.
         * Enforces bound checking.
         *
         * @tparam T     ubyte_t by default.
         *
         * @param  value Value to be written.
         *
         * @return True if the operation was successful.
         *         Depends on available space before the limit is reached.
         */
        template < typename T = ubyte_t >
        bool putr( const T value ) ; 


        /**
         * Puts `n` * T values relatively to the current position.
         * Advances position to the next byte after what is written
         * Enforces bound checking.
         *
         * @tparam T ubyte_t by default
         *
         * @param  values Immutable Tptr to the values.
         * @param  n      How many T has to be written.
         *                Defaults to 1.
         *
         * @return 0     if write is successful.
         *         n > 0 how many ubyte_t are missing for the write to be successful.
         */
        template < typename T = ubyte_t >
        uint16_t putr( const T *const  values      ,
                       const uint16_t  n      = 1  ) ;

        /**
         * Puts a T value at an absolute position within the limit.
         * Does not affect the current position.
         * Enforces bound checking.
         *
         * @paramt T     ubyte_t by default
         *
         * @param  pos   Absolute position in the buffer.
         * @param  value T value to write.
         *
         * @return True if the write was successful.
         */
        template < typename T = ubyte_t >
        bool puta( const uint16_t pos   ,
                   const T        value ) ;

        /**
         * Puts `n` * T values at an absolute position within the limit.
         * Does not affect the current position.
         * Enforces bound checking.
         *
         * @tparam T      ubyte_t by default.
         *
         * @param  pos    Absolute position in the buffer.
         * @param  values Immutable Tptr to the values.
         * @param  n      How many T has to be written.
         *                Default to 1.
         *
         * @return 0      if write was successful.
         *         n > 0  how many ubyte_t are missing for the write to be successful.
         */
        template < typename T = ubyte_t >
        uint16_t puta( const uint16_t pos         ,
                       const T *const values      ,
                       const uint16_t n      = 1  ) ;
    } ;




#include "byte_buffer_impl.cpp"

#endif
