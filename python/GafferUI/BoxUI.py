##########################################################################
#
#  Copyright (c) 2013-2015, Image Engine Design Inc. All rights reserved.
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
import functools

import IECore

import Gaffer
import GafferUI

Gaffer.Metadata.registerNode(

	Gaffer.Box,

	"description",
	"""
	A container for "subgraphs" - node networks which exist inside the
	Box and can be exposed by promoting selected internal plugs onto the
	outside of the Box.

	Boxes can be used as an organisational tool for simplifying large
	graphs by collapsing them into sections which perform distinct tasks.
	They are also used for authoring files to be used with the Reference
	node.
	""",

	"icon", "boxNode.png",

	"graphEditor:childrenViewable", True,

	# Add a + button for creating new plugs in the Settings tab.
	"layout:customWidget:addButton:widgetType", "GafferUI.UserPlugs.plugCreationWidget",
	"layout:customWidget:addButton:section", "Settings",
	"layout:customWidget:addButton:index", -2,

	# Add + buttons for creating new plugs in the GraphEditor
	"noduleLayout:customGadget:addButtonTop:gadgetType", "GafferUI.BoxUI.PlugAdder",
	"noduleLayout:customGadget:addButtonTop:section", "top",
	"noduleLayout:customGadget:addButtonBottom:gadgetType", "GafferUI.BoxUI.PlugAdder",
	"noduleLayout:customGadget:addButtonBottom:section", "bottom",
	"noduleLayout:customGadget:addButtonLeft:gadgetType", "GafferUI.BoxUI.PlugAdder",
	"noduleLayout:customGadget:addButtonLeft:section", "left",
	"noduleLayout:customGadget:addButtonRight:gadgetType", "GafferUI.BoxUI.PlugAdder",
	"noduleLayout:customGadget:addButtonRight:section", "right",

	plugs = {

		"*" : [

			"labelPlugValueWidget:renameable", True,

		],

		"user" : [

			# Disable the + button added by NodeUI, since
			# we want users to use the button in the Settings
			# tab instead.
			"layout:customWidget:addButton:widgetType", "",

		],

	}

)

##########################################################################
# Public functions
##########################################################################

## A command suitable for use with NodeMenu.append(), to add a menu
# item for the creation of a Box from the current selection. We don't
# actually append it automatically, but instead let the startup files
# for particular applications append it if it suits their purposes.
def nodeMenuCreateCommand( menu ) :

	graphEditor = menu.ancestor( GafferUI.GraphEditor )
	assert( graphEditor is not None )

	script = graphEditor.scriptNode()
	graphGadget = graphEditor.graphGadget()

	box = Gaffer.Box.create( graphGadget.getRoot(), script.selection() )
	Gaffer.BoxIO.insert( box )
	return box

## \deprecated Use GraphEditor.appendSubGraphMenuDefinitions()
def appendNodeContextMenuDefinitions( graphEditor, node, menuDefinition ) :

	if not isinstance( node, Gaffer.Box ) :
		return

	menuDefinition.append( "/BoxDivider", { "divider" : True } )
	menuDefinition.append( "/Show Contents...", { "command" : functools.partial( __showContents, graphEditor, node ) } )

## A callback suitable for use with NodeEditor.toolMenuSignal() - it provides
# menu options specific to Boxes. We don't actually register it automatically,
# but instead let the startup files for particular applications register
# it if it suits their purposes.
def appendNodeEditorToolMenuDefinitions( nodeEditor, node, menuDefinition ) :

	if not isinstance( node, Gaffer.Box ) :
		return

	menuDefinition.append( "/BoxDivider", { "divider" : True } )
	menuDefinition.append( "/Export reference...", { "command" : functools.partial( __exportForReferencing, node = node ) } )
	menuDefinition.append( "/Import reference...", { "command" : functools.partial( __importReference, node = node ) } )

	if Gaffer.BoxIO.canInsert( node ) :
		menuDefinition.append( "/UpgradeDivider", { "divider" : True } )
		menuDefinition.append( "/Upgrade to use BoxIO", { "command" : functools.partial( __upgradeToUseBoxIO, node = node ) } )

def __showContents( graphEditor, box ) :

	GafferUI.GraphEditor.acquire( box )

def __exportForReferencing( menu, node ) :

	bookmarks = GafferUI.Bookmarks.acquire( node, category="reference" )

	path = Gaffer.FileSystemPath( bookmarks.getDefault( menu ) )
	path.setFilter( Gaffer.FileSystemPath.createStandardFilter( [ "grf" ] ) )

	dialogue = GafferUI.PathChooserDialogue( path, title="Export reference", confirmLabel="Export", leaf=True, bookmarks=bookmarks )
	path = dialogue.waitForPath( parentWindow = menu.ancestor( GafferUI.Window ) )

	if not path :
		return

	path = str( path )
	if not path.endswith( ".grf" ) :
		path += ".grf"

	node.exportForReference( path )

def __importReference( menu, node ) :

	bookmarks = GafferUI.Bookmarks.acquire( node, category="reference" )

	path = Gaffer.FileSystemPath( bookmarks.getDefault( menu ) )
	path.setFilter( Gaffer.FileSystemPath.createStandardFilter( [ "grf" ] ) )

	window = menu.ancestor( GafferUI.Window )
	dialogue = GafferUI.PathChooserDialogue( path, title="Import reference", confirmLabel="Import", leaf=True, valid=True, bookmarks=bookmarks )
	path = dialogue.waitForPath( parentWindow = window )

	if not path :
		return

	scriptNode = node.ancestor( Gaffer.ScriptNode )
	with GafferUI.ErrorDialogue.ErrorHandler(
		title = "Error Importing Reference",
		closeLabel = "Oy vey",
		parentWindow = window
	) :
		with Gaffer.UndoScope( scriptNode ) :
			scriptNode.executeFile( str( path ), parent = node, continueOnError = True )

def __upgradeToUseBoxIO( node ) :

	with Gaffer.UndoScope( node.scriptNode() ) :
		Gaffer.BoxIO.insert( node )

# Shared menu code
##########################################################################

def __deletePlug( plug ) :

	with Gaffer.UndoScope( plug.ancestor( Gaffer.ScriptNode ) ) :
		plug.parent().removeChild( plug )

def __appendPlugDeletionMenuItems( menuDefinition, plug, readOnly = False ) :

	if not isinstance( plug.parent(), Gaffer.Box ) :
		return

	menuDefinition.append( "/DeleteDivider", { "divider" : True } )
	menuDefinition.append( "/Delete", {
		"command" : functools.partial( __deletePlug, plug ),
		"active" : not readOnly,
	} )

def __promote( plug ) :

	with Gaffer.UndoScope( plug.ancestor( Gaffer.ScriptNode ) ) :
		Gaffer.BoxIO.promote( plug )

def __unpromote( plug ) :

	with Gaffer.UndoScope( plug.ancestor( Gaffer.ScriptNode ) ) :
		Gaffer.PlugAlgo.unpromote( plug )

def __appendPlugPromotionMenuItems( menuDefinition, plug, readOnly = False ) :

	node = plug.node()
	if node is None :
		return

	box = node.ancestor( Gaffer.Box )
	if box is None :
		return

	parentLabel = {
		Gaffer.ArrayPlug : "Array",
		Gaffer.TransformPlug : "Transform",
		Gaffer.Transform2DPlug : "Transform",
	}.get( type( plug.parent() ) )

	if Gaffer.PlugAlgo.canPromote( plug ) :

		if len( menuDefinition.items() ) :
			menuDefinition.append( "/BoxDivider", { "divider" : True } )

		menuDefinition.append( "/Promote to %s" % box.getName(), {
			"command" : functools.partial( __promote, plug ),
			"active" : not readOnly,
		} )

		if parentLabel and Gaffer.PlugAlgo.canPromote( plug.parent() ) :
			menuDefinition.append( "/Promote %s to %s" % ( parentLabel, box.getName() ), {
				"command" : functools.partial( __promote, plug.parent() ),
				"active" : not readOnly,
			} )

	elif Gaffer.PlugAlgo.isPromoted( plug ) :

		# Add a menu item to unpromote the plug

		if len( menuDefinition.items() ) :
			menuDefinition.append( "/BoxDivider", { "divider" : True } )

		if parentLabel and Gaffer.PlugAlgo.isPromoted( plug.parent() ) :
			menuDefinition.append( "/Unpromote %s from %s" % ( parentLabel, box.getName() ), {
				"command" : functools.partial( __unpromote, plug.parent() ),
				"active" : not readOnly,
			} )
		else :
			# We dont want to allow unpromoting for individual children of promoted
			# parents because that would lead to ArrayPlugs and TransformPlugs with
			# the unexpected number of children, which would cause crashes.
			menuDefinition.append( "/Unpromote from %s" % box.getName(), {
				"command" : functools.partial( __unpromote, plug ),
				"active" : not readOnly,
			} )

# PlugValueWidget menu
##########################################################################

def __plugPopupMenu( menuDefinition, plugValueWidget ) :

	__appendPlugDeletionMenuItems( menuDefinition, plugValueWidget.getPlug(), readOnly = plugValueWidget.getReadOnly() )
	__appendPlugPromotionMenuItems( menuDefinition, plugValueWidget.getPlug(), readOnly = plugValueWidget.getReadOnly() )

__plugPopupMenuConnection = GafferUI.PlugValueWidget.popupMenuSignal().connect( __plugPopupMenu )

# GraphEditor plug context menu
##########################################################################

def __renamePlug( menu, plug ) :

	d = GafferUI.TextInputDialogue( initialText = plug.getName(), title = "Enter name", confirmLabel = "Rename" )
	name = d.waitForText( parentWindow = menu.ancestor( GafferUI.Window ) )

	if not name :
		return

	with Gaffer.UndoScope( plug.ancestor( Gaffer.ScriptNode ) ) :
		plug.setName( name )

def __setPlugMetadata( plug, key, value ) :

	with Gaffer.UndoScope( plug.ancestor( Gaffer.ScriptNode ) ) :
		Gaffer.Metadata.registerValue( plug, key, value )

def __edgePlugs( graphEditor, plug ) :

	nodeGadget = graphEditor.graphGadget().nodeGadget( plug.node() )
	nodule = nodeGadget.nodule( plug )
	return [ n.plug() for n in nodule.parent().children( GafferUI.Nodule ) ]

def __reorderPlugs( plugs, plug, newIndex ) :

	plugs.remove( plug )
	plugs.insert( newIndex, plug )
	with Gaffer.UndoScope( plug.ancestor( Gaffer.ScriptNode ) ) :
		for index, plug in enumerate( plugs ) :
			Gaffer.Metadata.registerValue( plug, "noduleLayout:index", index )

def __graphEditorPlugContextMenu( graphEditor, plug, menuDefinition ) :

	# The context menu might be showing for the child nodule
	# of a CompoundNodule, but most of our operations only
	# make sense on the top level parent plug, so find that
	# and use it.
	parentPlug = plug
	while isinstance( parentPlug.parent(), Gaffer.Plug ) :
		parentPlug = parentPlug.parent()

	readOnly = Gaffer.MetadataAlgo.readOnly( plug )

	if isinstance( plug.node(), Gaffer.Box ) :

		menuDefinition.append(
			"/Rename...",
			{
				"command" : functools.partial( __renamePlug, plug = parentPlug ),
				"active" : not readOnly,
			}
		)

		menuDefinition.append( "/MoveDivider", { "divider" : True } )

		currentEdge = Gaffer.Metadata.value( parentPlug, "noduleLayout:section" )
		if not currentEdge :
			currentEdge = "top" if parentPlug.direction() == parentPlug.Direction.In else "bottom"

		for edge in ( "top", "bottom", "left", "right" ) :
			menuDefinition.append(
				"/Move To/" + edge.capitalize(),
				{
					"command" : functools.partial( __setPlugMetadata, parentPlug, "noduleLayout:section", edge ),
					"active" : edge != currentEdge and not readOnly,
				}
			)

		edgePlugs = __edgePlugs( graphEditor, parentPlug )
		edgeIndex = edgePlugs.index( parentPlug )
		menuDefinition.append(
			"/Move " + ( "Up" if currentEdge in ( "left", "right" ) else "Left" ),
			{
				"command" : functools.partial( __reorderPlugs, edgePlugs, parentPlug, edgeIndex - 1 ),
				"active" : edgeIndex > 0 and not readOnly,
			}
		)

		menuDefinition.append(
			"/Move " + ( "Down" if currentEdge in ( "left", "right" ) else "Right" ),
			{
				"command" : functools.partial( __reorderPlugs, edgePlugs, parentPlug, edgeIndex + 1 ),
				"active" : edgeIndex < len( edgePlugs ) - 1 and not readOnly,
			}
		)

	__appendPlugDeletionMenuItems( menuDefinition, parentPlug, readOnly )
	__appendPlugPromotionMenuItems( menuDefinition, plug, readOnly )

GafferUI.GraphEditor.plugContextMenuSignal().connect( __graphEditorPlugContextMenu, scoped = False )
