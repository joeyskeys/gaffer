##########################################################################
#
#  Copyright (c) 2019, John Haddon. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are
#  met:
#
#      * Redistributions of source code must retain the above
#        copyright notice, this list of conditions and the following
#        disclaimer.
#
#      * Redistributions in binary form must reproduce the above
#        copyright notice, this list of conditions and the following
#        disclaimer in the documentation and/or other materials provided with
#        the distribution.
#
#      * Neither the name of John Haddon nor the names of
#        any other contributors to this software may be used to endorse or
#        promote products derived from this software without specific prior
#        written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
#  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
##########################################################################

import os
import sys
import unittest
import functools

import Gaffer
import GafferTest

class ExtensionAlgoTest( GafferTest.TestCase ) :

	def setUp( self ) :

		GafferTest.TestCase.setUp( self )

		self.addCleanup(
			functools.partial( setattr, sys, "path", sys.path[:] )
		)

	def testExport( self ) :

		box = Gaffer.Box( "AddOne" )

		box["__add"] = GafferTest.AddNode()
		box["__add"]["op2"].setValue( 1 )
		Gaffer.PlugAlgo.promote( box["__add"]["op1"] ).setName( "in" )
		Gaffer.PlugAlgo.promote( box["__add"]["sum"] ).setName( "out" )

		Gaffer.Metadata.registerValue( box, "description", "Test" )
		Gaffer.Metadata.registerValue( box["in"], "description", "The input" )
		Gaffer.Metadata.registerValue( box["out"], "description", "The output" )
		Gaffer.Metadata.registerValue( box["in"], "test", 1 )

		Gaffer.ExtensionAlgo.exportExtension( "TestExtension", [ box ], self.temporaryDirectory() )
		self.assertTrue( os.path.exists( os.path.join( self.temporaryDirectory(), "python", "TestExtension" ) ) )

		sys.path.append( os.path.join( self.temporaryDirectory(), "python" ) )

		import TestExtension

		node = TestExtension.AddOne()
		node["in"].setValue( 2 )
		self.assertEqual( node["out"].getValue(), 3 )

		import TestExtensionUI

		self.assertEqual( Gaffer.Metadata.registeredValues( node, instanceOnly = True ), [] )
		self.assertEqual( Gaffer.Metadata.registeredValues( node["in"], instanceOnly = True ), [] )
		self.assertEqual( Gaffer.Metadata.registeredValues( node["out"], instanceOnly = True ), [] )

		self.assertEqual( Gaffer.Metadata.value( node, "description" ), "Test" )
		self.assertEqual( Gaffer.Metadata.value( node["in"], "description" ), "The input" )
		self.assertEqual( Gaffer.Metadata.value( node["out"], "description" ), "The output" )
		self.assertEqual( Gaffer.Metadata.value( node["in"], "test" ), 1 )

if __name__ == "__main__":
	unittest.main()
