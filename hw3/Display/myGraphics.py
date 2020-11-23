# Luu, Hoang
# hxl9998
# 2020-10-25
#----------------------------------------------------------------------
from pathlib import Path

import tkinter as tk

from tkinter        import simpledialog
from tkinter        import filedialog

from DrawData       import DrawData

#----------------------------------------------------------------------
class world :
  def __init__( self, rootWindow ) :
    self.rootWindow = rootWindow
    self.widgets    = _widgets( self )
    self.canvas     = self.widgets.frame.canvas

  def reset( self ) :
    self.canvas.delete( 'all' )

  def drawData( self, drawData ) :
    for line in drawData.getLines() :
      self.canvas.create_line( *line )

    for polygon in drawData.getPolygons() :
      self.canvas.create_line( *polygon, *polygon[0:2] )

#----------------------------------------------------------------------
class _frame :
  def __init__( self, world ) :
    self.statusBar = world.statusBar

    self.canvas = tk.Canvas(
      world.rootWindow, width=1, height=1, bg='#FFC0CB' )

    self.canvas.pack( expand=tk.YES, fill=tk.BOTH )

  def canvas_resized_callback( self, event ) :
    self.resizeCanvas( event.width-4, event.height-4 )

  def resizeCanvas( self, width, height ) :
    self.canvas.winfo_toplevel().wm_geometry( '' )

    self.canvas.config( width = width, height = height )

    self.statusBar.pack( side = tk.BOTTOM, fill = tk.X )
    self.statusBar.set(
      f'Canvas width, height ({self.canvas.cget( "width" )}, ' +
      f'{self.canvas.cget( "height" )})' )

    self.canvas.pack()

#----------------------------------------------------------------------
class _menu :
  def __init__( self, world ) :
    self.world      = world
    self.rootWindow = world.rootWindow
    self.frame      = world.frame
    self.canvas     = world.frame.canvas
    self.statusBar  = world.statusBar

    self.menu = tk.Menu( self.rootWindow )
    self.rootWindow.config( menu = self.menu )

    dummy = tk.Menu( self.menu )
    self.menu.add_cascade( label = 'File', menu = dummy )
    dummy.add_command( label = 'Open...', command = lambda : self.menu_open_callback() )
    dummy.add_separator()
    dummy.add_command( label = 'Clear', command = lambda : self.menu_clear_callback() )

  def menu_callback( self, which = None ) :
    item = 'menu' if which is None else which
    self.statusBar.set( f'{item!r} callback' )

  def menu_clear_callback( self ) :
    self.world.reset()

  def menu_open_callback( self ) :
    fName = tk.filedialog.askopenfilename( filetypes = [ ( "Simple Draw File", "*.sdf" ) ] )
    if ( len( fName ) == 0 ) :
        self.statusBar_frame.set( "%s", "[Load was cancelled]" )
        return

    drawData = DrawData( fName )
    drawData.dump()
    w = drawData.getWidth()
    h = drawData.getHeight()

    self.frame.resizeCanvas( w, h )
    self.canvas.create_line( 1, 1, w, 1, w, h, 1, h, 1, 1, fill='white' )

    self.world.drawData( drawData )

    baseFileName = Path( fName ).name
    self.statusBar.set( f'Loaded {baseFileName!r}' )

#----------------------------------------------------------------------
class _statusBar( tk.Frame ) :
  def __init__( self, rootWindow ) :
    tk.Frame.__init__( self, rootWindow )

    self.label = tk.Label( self, bd = 1, relief = tk.SUNKEN, anchor = tk.W )
    self.label.pack( fill = tk.X )

  def set( self, formatStr, *args ) :
    msg = f'hxl9998: {formatStr % args}'
    print( msg )
    self.label.config( text = msg )

    self.label.update_idletasks()

  def clear( self ) :
    self.label.config( text='' )
    self.label.update_idletasks()

#----------------------------------------------------------------------
class _widgets :
  def __init__( self, world ) :
    self.world      = world
    self.rootWindow = world.rootWindow

    self.statusBar = _statusBar( self.rootWindow )
    self.statusBar.pack( side = tk.BOTTOM, fill = tk.X )
    self.statusBar.set( 'This is the status bar' )
    world.statusBar = self.statusBar

    self.frame     = _frame( world )
    world.frame    = self.frame

    self.menu      = _menu( world )

#----------------------------------------------------------------------
