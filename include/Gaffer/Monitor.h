//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2016, Image Engine Design Inc. All rights reserved.
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

#ifndef GAFFER_MONITOR_H
#define GAFFER_MONITOR_H

#include "Gaffer/Export.h"
#include "Gaffer/ThreadState.h"

#include "IECore/RefCounted.h"

namespace Gaffer
{

class Process;

/// Base class for monitoring node graph processes.
class GAFFER_API Monitor : public IECore::RefCounted
{

	public :

		virtual ~Monitor();

		IE_CORE_DECLAREMEMBERPTR( Monitor )

		using MonitorSet = boost::container::flat_set<MonitorPtr>;

		class Scope : private ThreadState::Scope
		{

			public :

				/// Constructs a Scope where the monitor has the specified
				/// active state. If monitor is null, the scope is a no-op.
				Scope( const MonitorPtr &monitor, bool active = true );
				/// Constructs a Scope where each of `monitors` has the
				/// specified `active` state.
				Scope( const MonitorSet &monitors, bool active = true );
				/// Returns to the previously active set of monitors.
				~Scope();

			private :

				MonitorSet m_monitors;

		};

		/// Returns the set of monitors that are currently active
		/// on this thread.
		static const MonitorSet &current();

	protected :

		Monitor();

		friend class Process;

		/// Implementations must be safe to call concurrently.
		virtual void processStarted( const Process *process ) = 0;
		/// Implementations must be safe to call concurrently.
		virtual void processFinished( const Process *process ) = 0;

};

IE_CORE_DECLAREPTR( Monitor )

} // namespace Gaffer

#endif // GAFFER_MONITOR_H
