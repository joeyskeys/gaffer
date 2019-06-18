##########################################################################
#
#  Copyright (c) 2011-2013, John Haddon. All rights reserved.
#  Copyright (c) 2011-2013, Image Engine Design Inc. All rights reserved.
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

import weakref

import Gaffer
import GafferUI

from Qt import QtCore

class ColorPlugValueWidget( GafferUI.CompoundNumericPlugValueWidget ) :

	def __init__( self, plug, **kw ) :

		GafferUI.CompoundNumericPlugValueWidget.__init__( self, plug, **kw )

		self.__swatch = GafferUI.ColorSwatchPlugValueWidget( plug )

		self._row().append( self.__swatch, expand=True )

		self.__swatch.buttonReleaseSignal().connect( Gaffer.WeakMethod( self.__buttonRelease ), scoped = False )

		self.__blinkBehaviour = None

	def setPlug( self, plug ) :

		GafferUI.CompoundNumericPlugValueWidget.setPlug( self, plug )

		self.__swatch.setPlug( plug )

	def __buttonRelease( self, widget, event ) :

		if not self._editable() :

			# the swatch will have been unable to display a colour chooser, so we
			# draw the user's attention to the components which are preventing that.
			if self.__blinkBehaviour is not None :
				self.__blinkBehaviour.stop()
			widgets = [ w for w in self._row()[:len( self.getPlug() )] if not w._editable() ]
			self.__blinkBehaviour = _BlinkBehaviour( widgets )
			self.__blinkBehaviour.start()

			return False

GafferUI.PlugValueWidget.registerType( Gaffer.Color3fPlug, ColorPlugValueWidget )
GafferUI.PlugValueWidget.registerType( Gaffer.Color4fPlug, ColorPlugValueWidget )

## \todo Consider if this is something that might be useful elsewhere, if
# there are other such things, and what a Behaviour base class for them
# might look like.
class _BlinkBehaviour( object ) :

	def __init__( self, targetWidgets, blinks = 2 ) :

		self.__targetWidgets = [ weakref.ref( w ) for w in targetWidgets ]
		self.__initialStates = [ w.getHighlighted() for w in targetWidgets ]

		self.__blinks = blinks
		self.__toggleCount = 0
		self.__timer = QtCore.QTimer()
		self.__timer.timeout.connect( self.__blink )

	def start( self ) :

		self.__toggleCount = 0
		self.__blink()
		self.__timer.start( 250 )

	def stop( self ) :

		self.__timer.stop()
		for widget, initialState in zip( self.__targetWidgets, self.__initialStates ) :
			widget = widget()
			if widget :
				widget.setHighlighted( initialState )

	def __blink( self ) :

		self.__toggleCount += 1

		for widget, initialState in zip( self.__targetWidgets, self.__initialStates ) :
			widget = widget()
			if widget :
				widget.setHighlighted( bool( ( int( initialState ) + self.__toggleCount ) % 2 ) )

		if self.__toggleCount >= self.__blinks * 2 :
			self.__timer.stop()
