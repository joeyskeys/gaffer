//////////////////////////////////////////////////////////////////////////
//  
//  Copyright (c) 2012, John Haddon. All rights reserved.
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

#ifndef GAFFERSCENE_INSTANCER_H
#define GAFFERSCENE_INSTANCER_H

#include "GafferScene/BranchCreator.h"

namespace GafferScene
{

class Instancer : public BranchCreator
{

	public :

		Instancer( const std::string &name=staticTypeName() );
		virtual ~Instancer();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Instancer, InstancerTypeId, BranchCreator );
		
		ScenePlug *instancePlug();
		const ScenePlug *instancePlug() const;
		
		virtual void affects( const Gaffer::ValuePlug *input, AffectedPlugsContainer &outputs ) const;

	protected :
	
		virtual Imath::Box3f computeBranchBound( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context ) const;
		virtual Imath::M44f computeBranchTransform( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context ) const;
		virtual IECore::ObjectPtr computeBranchObject( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context ) const;
		virtual IECore::StringVectorDataPtr computeBranchChildNames( const ScenePath &parentPath, const ScenePath &branchPath, const Gaffer::Context *context ) const;
		
	private :
	
		IECore::ConstV3fVectorDataPtr sourcePoints( const ScenePath &parentPath ) const;
		int instanceIndex( const ScenePath &branchPath ) const;
		Gaffer::ContextPtr instanceContext( const Gaffer::Context *parentContext, const ScenePath &branchPath ) const;
		
};

} // namespace GafferScene

#endif // GAFFERSCENE_INSTANCER_H
