//By Peter Dimov, example code from propsal N2179

#include "exception_ptr.hpp"
#include <exception>
#include <stdexcept>
#include <ios> //for ios_base::failure declaration
#include <boost/thread/exceptions.hpp>
#include <boost/version.hpp>
#include "future.hpp"
#include "future_exceptions.hpp"

namespace boost {
  namespace detail {
    class _exp_throwable
    {
      protected:

        virtual ~_exp_throwable() {}

      public:

        virtual void rethrow() = 0;
    };

    template< class E > class _exp_throwable_impl: public _exp_throwable
    {
      private:

        E e_;

      public:

        _exp_throwable_impl()
        {
        }

        template< class A > _exp_throwable_impl( A const & a ): e_( a )
      {
      }

        void rethrow()
        {
          throw e_;
        }
        virtual ~_exp_throwable_impl() {}
    };

#define _CATCH_AND_RETURN( E ) catch( E const & e ) { return exception_ptr( new _exp_throwable_impl< E >( e ) ); }

    inline static exception_ptr _exp_current_exception()
    {
      try
      {
        throw;
      }

      // runtime_error standard subclasses
      _CATCH_AND_RETURN( std::overflow_error )
        _CATCH_AND_RETURN( std::range_error )
        _CATCH_AND_RETURN( std::underflow_error)

        _CATCH_AND_RETURN( std::runtime_error )

        // logic_error standard subclasses
        _CATCH_AND_RETURN( std::domain_error )
        _CATCH_AND_RETURN( std::invalid_argument )
        _CATCH_AND_RETURN( std::length_error )
        _CATCH_AND_RETURN( std::out_of_range )
        _CATCH_AND_RETURN( std::logic_error )

        _CATCH_AND_RETURN( std::bad_alloc )
        _CATCH_AND_RETURN( std::bad_cast )
        _CATCH_AND_RETURN( std::bad_typeid )
        _CATCH_AND_RETURN( std::bad_exception )

        // iostreams library
        _CATCH_AND_RETURN( std::ios_base::failure )

        // boost.thread exceptions
        _CATCH_AND_RETURN( boost::lock_error )
        _CATCH_AND_RETURN( boost::thread_resource_error )
        _CATCH_AND_RETURN( boost::unsupported_thread_option )
        _CATCH_AND_RETURN( boost::thread_permission_error )
        _CATCH_AND_RETURN( boost::invalid_thread_argument )
        _CATCH_AND_RETURN( boost::thread_exception )
#if BOOST_VERSION >= 103500
        _CATCH_AND_RETURN( boost::thread_interrupted ) // boost 1.5
#endif
        _CATCH_AND_RETURN( boost::broken_promise )
        _CATCH_AND_RETURN( boost::future_already_set )
        _CATCH_AND_RETURN( boost::future_cancel )

        catch( std::exception const & e )
        {
          return exception_ptr( new _exp_throwable_impl<std::runtime_error>( e.what() ) );
        }
      catch( ... )
      {
        return exception_ptr( new _exp_throwable_impl<std::bad_exception>() );
      }
    }

    // FIXME: can't have this global static as header-only. -braddock
    //static exception_ptr s_bad_alloc( new _exp_throwable_impl< std::bad_alloc > );

    inline exception_ptr current_exception()
    {
      try
      {
        return _exp_current_exception();
      }
      catch( std::bad_alloc const & )
      {
        // FIXME: see above note about static s_bad_alloc in header-only -braddock
        //return s_bad_alloc;
        return exception_ptr( new _exp_throwable_impl< std::bad_alloc > ); //PROBABLY WILL FAIL -braddock
      }
    }

    inline void rethrow_exception( exception_ptr p )
    {
      p->rethrow();
    }

  } //detail
} //namespace boost

