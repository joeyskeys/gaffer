//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2011-2013, John Haddon. All rights reserved.
//  Copyright (c) 2011, Image Engine Design Inc. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//      * Redistributions of source code must retain the above
//        copyright notice, this list of conditions and the following
//        disclaimer.
//
//      * Redistributions in binary form must reproduce the above
//        copyright notice, this list of conditions and the following
//        disclaimer in the documentation and/or other materials provided with
//        the distribution.
//
//      * Neither the name of John Haddon nor the names of
//        any other contributors to this software may be used to endorse or
//        promote products derived from this software without specific prior
//        written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#ifndef GAFFERBINDINGS_SIGNALBINDING_INL
#define GAFFERBINDINGS_SIGNALBINDING_INL

#include "IECorePython/ExceptionAlgo.h"
#include "IECorePython/ScopedGILLock.h"
#include "IECorePython/ScopedGILRelease.h"

#include "boost/signals.hpp"
#include "boost/version.hpp"

namespace GafferBindings
{

namespace Detail
{

template<int Arity, typename Signal>
struct DefaultSignalCallerBase;

template<typename Signal>
struct DefaultSignalCallerBase<0, Signal>
{
	static typename Signal::result_type call( Signal &s )
	{
		IECorePython::ScopedGILRelease gilRelease;
		return s();
	}
};

template<typename Signal>
struct DefaultSignalCallerBase<1, Signal>
{
#if BOOST_VERSION < 103900
	static typename Signal::result_type call( Signal &s, typename Signal::arg2_type a1 )
#else
	static typename Signal::result_type call( Signal &s, typename Signal::arg1_type a1 )
#endif
	{
		IECorePython::ScopedGILRelease gilRelease;
		return s( a1 );
	}
};

template<typename Signal>
struct DefaultSignalCallerBase<2, Signal>
{
#if BOOST_VERSION < 103900
	static typename Signal::result_type call( Signal &s, typename Signal::arg2_type a1, typename Signal::arg3_type a2 )
#else
	static typename Signal::result_type call( Signal &s, typename Signal::arg1_type a1, typename Signal::arg2_type a2 )
#endif
	{
		IECorePython::ScopedGILRelease gilRelease;
		return s( a1, a2 );
	}
};

template<typename Signal>
struct DefaultSignalCallerBase<3, Signal>
{
#if BOOST_VERSION < 103900
	static typename Signal::result_type call( Signal &s, typename Signal::arg2_type a1, typename Signal::arg3_type a2, typename Signal::arg4_type a3 )
#else
	static typename Signal::result_type call( Signal &s, typename Signal::arg1_type a1, typename Signal::arg2_type a2, typename Signal::arg3_type a3 )
#endif
	{
		IECorePython::ScopedGILRelease gilRelease;
		return s( a1, a2, a3 );
	}
};

template<typename Signal>
struct DefaultSignalCallerBase<4, Signal>
{
#if BOOST_VERSION < 103900
	static typename Signal::result_type call( Signal &s, typename Signal::arg2_type a1, typename Signal::arg3_type a2, typename Signal::arg4_type a3, typename Signal::arg5_type a4 )
#else
	static typename Signal::result_type call( Signal &s, typename Signal::arg1_type a1, typename Signal::arg2_type a2, typename Signal::arg3_type a3, typename Signal::arg4_type a4 )
#endif
	{
		IECorePython::ScopedGILRelease gilRelease;
		return s( a1, a2, a3, a4 );
	}
};

template<int Arity, typename Signal>
struct DefaultSlotCallerBase;

template<typename Signal>
struct DefaultSlotCallerBase<0, Signal>
{
	typename Signal::slot_result_type operator()( boost::python::object slot )
	{
		return boost::python::extract<typename Signal::slot_result_type>( slot() )();
	}
};

template<typename Signal>
struct DefaultSlotCallerBase<1, Signal>
{
#if BOOST_VERSION < 103900
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg2_type a1 )
#else
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg1_type a1 )
#endif
	{
		return boost::python::extract<typename Signal::slot_result_type>( slot( a1 ) )();
	}
};

template<typename Signal>
struct DefaultSlotCallerBase<2, Signal>
{
#if BOOST_VERSION < 103900
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg2_type a1, typename Signal::arg3_type a2 )
#else
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg1_type a1, typename Signal::arg2_type a2 )
#endif
	{
		return boost::python::extract<typename Signal::slot_result_type>( slot( a1, a2 ) )();
	}
};

template<typename Signal>
struct DefaultSlotCallerBase<3, Signal>
{
#if BOOST_VERSION < 103900
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg2_type a1, typename Signal::arg3_type a2, typename Signal::arg4_type a3 )
#else
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg1_type a1, typename Signal::arg2_type a2, typename Signal::arg3_type a3 )
#endif
	{
		return boost::python::extract<typename Signal::slot_result_type>( slot( a1, a2, a3 ) )();
	}
};

template<typename Signal>
struct DefaultSlotCallerBase<4, Signal>
{
#if BOOST_VERSION < 103900
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg2_type a1, typename Signal::arg3_type a2, typename Signal::arg4_type a3, typename Signal::arg5_type a4 )
#else
	typename Signal::slot_result_type operator()( boost::python::object slot, typename Signal::arg1_type a1, typename Signal::arg2_type a2, typename Signal::arg3_type a3, typename Signal::arg4_type a4 )
#endif
	{
		return boost::python::extract<typename Signal::slot_result_type>( slot( a1, a2, a3, a4 ) )();
	}
};

template<int Arity, typename Signal, typename Caller>
struct SlotBase;

template<typename Signal, typename Caller>
struct SlotBase<0, Signal, Caller>
{
	SlotBase( boost::python::object slot )
		:	m_slot( boost::python::borrowed( slot.ptr() ) )
	{
	}
	~SlotBase()
	{
		IECorePython::ScopedGILLock gilLock;
		m_slot.reset();
	}
	typename Signal::slot_result_type operator()()
	{
		IECorePython::ScopedGILLock gilLock;
		try
		{
			return Caller()( boost::python::object( m_slot ) );
		}
		catch( const boost::python::error_already_set& e )
		{
			IECorePython::ExceptionAlgo::translatePythonException();
		}
		return typename Signal::slot_result_type();
	}
	boost::python::handle<PyObject> m_slot;
};

template<typename Signal, typename Caller>
struct SlotBase<1, Signal, Caller>
{
	SlotBase( boost::python::object slot )
		:	m_slot( boost::python::borrowed( slot.ptr() ) )
	{
	}
	~SlotBase()
	{
		IECorePython::ScopedGILLock gilLock;
		m_slot.reset();
	}
#if BOOST_VERSION < 103900
	typename Signal::slot_result_type operator()( typename Signal::arg2_type a1 )
#else
	typename Signal::slot_result_type operator()( typename Signal::arg1_type a1 )
#endif
	{
		IECorePython::ScopedGILLock gilLock;
		try
		{
			return Caller()( boost::python::object( m_slot ), a1 );
		}
		catch( const boost::python::error_already_set& e )
		{
			IECorePython::ExceptionAlgo::translatePythonException();
		}
		return typename Signal::slot_result_type();
	}
	boost::python::handle<PyObject> m_slot;
};

template<typename Signal, typename Caller>
struct SlotBase<2, Signal, Caller>
{
	SlotBase( boost::python::object slot )
		:	m_slot( boost::python::borrowed( slot.ptr() ) )
	{
	}
	~SlotBase()
	{
		IECorePython::ScopedGILLock gilLock;
		m_slot.reset();
	}
#if BOOST_VERSION < 103900
	typename Signal::slot_result_type operator()( typename Signal::arg2_type a1, typename Signal::arg3_type a2 )
#else
	typename Signal::slot_result_type operator()( typename Signal::arg1_type a1, typename Signal::arg2_type a2 )
#endif
	{
		IECorePython::ScopedGILLock gilLock;
		try
		{
			return Caller()( boost::python::object( m_slot ), a1, a2 );
		}
		catch( const boost::python::error_already_set& e )
		{
			IECorePython::ExceptionAlgo::translatePythonException();
		}
		return typename Signal::slot_result_type();
	}
	boost::python::handle<PyObject> m_slot;
};

template<typename Signal, typename Caller>
struct SlotBase<3, Signal, Caller>
{
	SlotBase( boost::python::object slot )
		:	m_slot( boost::python::borrowed( slot.ptr() ) )
	{
	}
	~SlotBase()
	{
		IECorePython::ScopedGILLock gilLock;
		m_slot.reset();
	}
#if BOOST_VERSION < 103900
	typename Signal::slot_result_type operator()( typename Signal::arg2_type a1, typename Signal::arg3_type a2, typename Signal::arg4_type a3 )
#else
	typename Signal::slot_result_type operator()( typename Signal::arg1_type a1, typename Signal::arg2_type a2, typename Signal::arg3_type a3 )
#endif
	{
		IECorePython::ScopedGILLock gilLock;
		try
		{
			return Caller()( boost::python::object( m_slot ), a1, a2, a3 );
		}
		catch( const boost::python::error_already_set& e )
		{
			IECorePython::ExceptionAlgo::translatePythonException();
		}
		return typename Signal::slot_result_type();
	}
	boost::python::handle<PyObject> m_slot;
};

template<typename Signal, typename Caller>
struct SlotBase<4, Signal, Caller>
{
	SlotBase( boost::python::object slot )
		:	m_slot( boost::python::borrowed( slot.ptr() ) )
	{
	}
	~SlotBase()
	{
		IECorePython::ScopedGILLock gilLock;
		m_slot.reset();
	}
#if BOOST_VERSION < 103900
	typename Signal::slot_result_type operator()( typename Signal::arg2_type a1, typename Signal::arg3_type a2, typename Signal::arg4_type a3, typename Signal::arg5_type a4 )
#else
	typename Signal::slot_result_type operator()( typename Signal::arg1_type a1, typename Signal::arg2_type a2, typename Signal::arg3_type a3, typename Signal::arg4_type a4 )
#endif
	{
		IECorePython::ScopedGILLock gilLock;
		try
		{
			return Caller()( boost::python::object( m_slot ), a1, a2, a3, a4 );
		}
		catch( const boost::python::error_already_set& e )
		{
			IECorePython::ExceptionAlgo::translatePythonException();
		}
		return typename Signal::slot_result_type();
	}
	boost::python::handle<PyObject> m_slot;
};

template<typename Signal, typename Caller>
struct Slot : public SlotBase<Signal::slot_function_type::arity, Signal, Caller>
{
	Slot( boost::python::object slot )
		:	SlotBase<Signal::slot_function_type::arity, Signal, Caller>( slot )
	{
	}
};

// Ideally we would bind `boost::signals::trackable` to Python
// directly, but its protected destructor prevents that. So we
// bind this little derived class instead.
struct Trackable : public boost::signals::trackable
{
};

// Overload boost's `visit_each()` function for all our Slot types.
// Boost will call this to discover slots which refer to trackable
// objects, and will use it to automatically remove the connection
// when the `trackable` object dies.
template<typename Visitor, typename Signal, typename Caller>
void visit_each( Visitor &visitor, const Slot<Signal, Caller> &slot, int )
{
	// Check to see if slot contains a WeakMethod referring to a trackable
	// object. There is no point checking for regular methods, because they
	// prevent the trackable object from dying until it has been disconnected
	// manually.
	boost::python::object gaffer = boost::python::import( "Gaffer" );
	boost::python::object weakMethod = gaffer.attr( "WeakMethod" );
	if( PyObject_IsInstance( slot.m_slot.get(), weakMethod.ptr() ) )
	{
		boost::python::object self = boost::python::object( slot.m_slot ).attr( "instance" )();
		boost::python::extract<Trackable &> e( self );
		if( e.check() )
		{
			boost::visit_each( visitor, e(), 0 );
		}
	}
}

GAFFERBINDINGS_API boost::python::object pythonConnection( const boost::signals::connection &connection, bool scoped );

template<typename Signal, typename SlotCaller>
boost::python::object connect( Signal &s, boost::python::object &slot, bool scoped )
{
	return pythonConnection( s.connect( Slot<Signal, SlotCaller>( slot ) ), scoped );
}

template<typename Signal, typename SlotCaller>
boost::python::object connectInGroup( Signal &s, int group, boost::python::object &slot, bool scoped )
{
	return pythonConnection( s.connect( group, Slot<Signal, SlotCaller>( slot ) ), scoped );
}

} // namespace Detail

template<typename Signal>
struct DefaultSignalCaller : public Detail::DefaultSignalCallerBase<Signal::slot_function_type::arity, Signal>
{

};

template<typename Signal>
struct DefaultSlotCaller : public Detail::DefaultSlotCallerBase<Signal::slot_function_type::arity, Signal>
{
};

template<typename Signal, typename SignalCaller, typename SlotCaller>
SignalClass<Signal, SignalCaller, SlotCaller>::SignalClass( const char *className, const char *docString )
	:	boost::python::class_<Signal, boost::noncopyable>( className, docString )
{
	this->def( "connect", &Detail::connect<Signal, SlotCaller>, ( boost::python::arg( "slot" ), boost::python::arg( "scoped" ) = true ) );
	this->def( "connect", &Detail::connectInGroup<Signal, SlotCaller>, ( boost::python::arg( "group" ), boost::python::arg( "slot" ), boost::python::arg( "scoped" ) = true ) );
	this->def( "num_slots", &Signal::num_slots );
	this->def( "empty", &Signal::empty );
	this->def( "__call__", &SignalCaller::call );
}

} // namespace GafferBindings

#endif // GAFFERBINDINGS_SIGNALBINDING_INL
