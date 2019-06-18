//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2019, Image Engine Design Inc. All rights reserved.
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

#ifndef GAFFERARNOLD_ARNOLDCAMERASHADERS_H
#define GAFFERARNOLD_ARNOLDCAMERASHADERS_H

#include "GafferArnold/Export.h"
#include "GafferArnold/TypeIds.h"

#include "GafferScene/Shader.h"
#include "GafferScene/ShaderPlug.h"

namespace GafferArnold
{

/// \todo See ArnoldDisplacement for comments regarding the
/// awkwardness of deriving from Shader, and the possibility
/// of making a more general Assignable class.
class GAFFERARNOLD_API ArnoldCameraShaders : public GafferScene::Shader
{

	public :

		ArnoldCameraShaders( const std::string &name=defaultName<ArnoldCameraShaders>() );
		~ArnoldCameraShaders() override;

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( GafferArnold::ArnoldCameraShaders, ArnoldCameraShadersTypeId, GafferScene::Shader );

		GafferScene::ShaderPlug *filterMapPlug();
		const GafferScene::ShaderPlug *filterMapPlug() const;

		GafferScene::ShaderPlug *uvRemapPlug();
		const GafferScene::ShaderPlug *uvRemapPlug() const;

		Gaffer::Plug *outPlug();
		const Gaffer::Plug *outPlug() const;

		void affects( const Gaffer::Plug *input, AffectedPlugsContainer &outputs ) const override;

	protected :

		void attributesHash( const Gaffer::Plug *output, IECore::MurmurHash &h ) const override;
		IECore::ConstCompoundObjectPtr attributes( const Gaffer::Plug *output ) const override;

		bool acceptsInput( const Gaffer::Plug *plug, const Gaffer::Plug *inputPlug ) const override;

	private :

		static size_t g_firstPlugIndex;

};

IE_CORE_DECLAREPTR( ArnoldCameraShaders )

} // namespace GafferArnold

#endif // GAFFERARNOLD_ARNOLDCAMERASHADERS_H
